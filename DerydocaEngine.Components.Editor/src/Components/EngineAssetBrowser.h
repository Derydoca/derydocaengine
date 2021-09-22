#pragma once
#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"

namespace DerydocaEngine::Components
{
	struct File : public Object
	{
		REGISTER_TYPE_ID(File);
		std::filesystem::path Path;
	};

	struct Directory
	{
		std::filesystem::path Path;
		std::vector<File> Files;
		std::vector<Directory> Children;
	};

	class EngineAssetBrowser : public EditorWindowComponent, SelfRegister<EngineAssetBrowser>
	{
	public:
		GENINSTANCE(EngineAssetBrowser);

		EngineAssetBrowser();
		~EngineAssetBrowser();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Asset Browser"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

		void renderFileSystemContent(Directory directory);
		void renderFolderNode(Directory directory);
		void renderFileNode(File file);

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::EditorWindowComponent);

	private:
		void refresh();
		void refreshDir(Directory& dir);

	private:
		std::weak_ptr<Resources::ResourceTreeNode> m_ResourceNode;
		std::shared_ptr<Editor::SelectionGroup> m_SelectionGroup;
		Directory m_RootDir;

	};
}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::EngineAssetBrowser, 0);
