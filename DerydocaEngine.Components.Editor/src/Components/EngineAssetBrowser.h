#pragma once
#include "Components\EditorWindowComponent.h"

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
	private:
		std::weak_ptr<Resources::ResourceTreeNode> m_resourceNode;
	};
}