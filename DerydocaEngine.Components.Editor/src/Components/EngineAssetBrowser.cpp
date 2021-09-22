#include "EditorComponentsPch.h"
#include "EngineAssetBrowser.h"
#include "ObjectLibrary.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Components
{

	EngineAssetBrowser::EngineAssetBrowser()
	{
		m_SelectionGroup = Editor::SelectionManager::getInstance().getPrimarySelectionGroup();

		refresh();
	}

	EngineAssetBrowser::~EngineAssetBrowser()
	{
	}

	void EngineAssetBrowser::renderWindow()
	{
		if (m_ResourceNode.expired())
		{
			m_ResourceNode = ObjectLibrary::getInstance().getRootResourceTreeNode();
			return;
		}

		auto root = m_ResourceNode.lock();

		renderFileSystemContent(m_RootDir);
	}

	void EngineAssetBrowser::renderFileSystemContent(Directory directory)
	{
		for (auto child : directory.Children)
		{
			renderFolderNode(child);
		}

		for (auto file : directory.Files)
		{
			renderFileNode(file);
		}
	}

	void EngineAssetBrowser::renderFolderNode(Directory directory)
	{
		if (ImGui::TreeNode(directory.Path.filename().string().c_str()))
		{
			renderFileSystemContent(directory);

			ImGui::TreePop();
		}
	}

	void EngineAssetBrowser::renderFileNode(File file)
	{
		//std::string id = resource->getId().to_string();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		//if (m_SelectionGroup->isSelected(file))
		//{
		//	flags |= ImGuiTreeNodeFlags_Selected;
		//}
		std::string pathStr = file.Path.string();
		std::string filenameStr = file.Path.filename().string();
		ImGui::TreeNodeEx(pathStr.c_str(), flags, "%s", filenameStr.c_str());
		if (ImGui::IsItemClicked())
		{
			//m_SelectionGroup->select(resource);
		}
	}

	void EngineAssetBrowser::refresh()
	{
		std::filesystem::path rootPath = ObjectLibrary::getInstance().getProjectDirectory();
		m_RootDir.Path = rootPath;
		refreshDir(m_RootDir);
	}

	void EngineAssetBrowser::refreshDir(Directory& dir)
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ dir.Path })
		{
			if (dir_entry.is_directory())
			{
				Directory childDir{};
				childDir.Path = dir_entry.path();

				refreshDir(childDir);

				dir.Children.push_back(childDir);
			}
			else
			{
				if (dir_entry.path().has_extension() && dir_entry.path().extension() == ".dmeta")
				{
					continue;
				}
				File f{};
				f.Path = dir_entry.path();
				dir.Files.push_back(f);
			}
		}
	}

}
