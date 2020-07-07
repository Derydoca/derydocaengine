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

		void renderNode(std::shared_ptr<Resources::ResourceTreeNode> node);
		void renderNodeContent(std::shared_ptr<Resources::ResourceTreeNode> node);
		void renderResourceNode(std::shared_ptr<Resources::Resource> resource);

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::EditorWindowComponent);
	private:
		std::weak_ptr<Resources::ResourceTreeNode> m_ResourceNode;
		std::shared_ptr<Editor::SelectionGroup> m_SelectionGroup;
	};
}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::EngineAssetBrowser);
