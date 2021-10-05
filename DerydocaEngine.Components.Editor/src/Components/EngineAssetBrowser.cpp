#include "EditorComponentsPch.h"
#include "EngineAssetBrowser.h"
#include "ObjectLibrary.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Components
{

	EngineAssetBrowser::EngineAssetBrowser()
	{
		m_SelectionGroup = Editor::SelectionManager::getInstance().getPrimarySelectionGroup();
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

		renderFileSystemContent(ObjectLibrary::getInstance().getProjectResourceDirectoryRoot());
	}

	void EngineAssetBrowser::renderFileSystemContent(std::shared_ptr<Directory> directory)
	{
		for (auto child : directory->Children)
		{
			renderFolderNode(child);
		}

		for (auto file : directory->Files)
		{
			renderFileNode(file);
		}
	}

	void EngineAssetBrowser::renderFolderNode(std::shared_ptr<Directory> directory)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (m_SelectionGroup->isSelected(directory))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		std::string pathStr = directory->Path.string();
		std::string filenameStr = directory->Path.filename().string();
		bool isOpen = ImGui::TreeNodeEx(pathStr.c_str(), flags, "%s", filenameStr.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionGroup->select(directory);
		}
		if (isOpen)
		{
			renderFileSystemContent(directory);
			ImGui::TreePop();
		}
	}

	void EngineAssetBrowser::renderFileNode(std::shared_ptr<File> file)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (m_SelectionGroup->isSelected(file))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		std::string pathStr = file->Path.string();
		std::string filenameStr = file->Path.filename().string();
		ImGui::TreeNodeEx(pathStr.c_str(), flags, "%s", filenameStr.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionGroup->select(file);
		}
	}

}
