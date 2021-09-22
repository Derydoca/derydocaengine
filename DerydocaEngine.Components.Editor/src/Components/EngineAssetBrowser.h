#pragma once
#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"

namespace DerydocaEngine::Components
{
	class EngineAssetBrowser : public EditorWindowComponent, SelfRegister<EngineAssetBrowser>
	{
	public:
		GENINSTANCE(EngineAssetBrowser);

		EngineAssetBrowser();
		~EngineAssetBrowser();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Asset Browser"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

		void renderFileSystemContent(std::shared_ptr<Directory> directory);
		void renderFolderNode(std::shared_ptr<Directory> directory);
		void renderFileNode(File file);

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::EditorWindowComponent);

	private:
		std::weak_ptr<Resources::ResourceTreeNode> m_ResourceNode;
		std::shared_ptr<Editor::SelectionGroup> m_SelectionGroup;

	};
}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::EngineAssetBrowser, 0);
