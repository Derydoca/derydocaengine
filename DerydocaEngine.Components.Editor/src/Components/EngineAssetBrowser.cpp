#include "EditorComponentsPch.h"
#include "EngineAssetBrowser.h"
#include "ObjectLibrary.h"

namespace DerydocaEngine::Components
{

	EngineAssetBrowser::EngineAssetBrowser()
	{
	}

	EngineAssetBrowser::~EngineAssetBrowser()
	{
	}

	void EngineAssetBrowser::renderWindow()
	{
		if (m_resourceNode.expired())
		{
			m_resourceNode = ObjectLibrary::getInstance().getRootResourceTreeNode();
			return;
		}

		auto root = m_resourceNode.lock();

		renderNodeContent(root);
	}

	void EngineAssetBrowser::renderNodeContent(std::shared_ptr<Resources::ResourceTreeNode> node)
	{
		for (auto child : node->getChildren())
		{
			renderNode(child);
		}

		for (auto resource : node->getResources())
		{
			renderResourceNode(resource);
		}
	}

	void EngineAssetBrowser::renderNode(std::shared_ptr<Resources::ResourceTreeNode> node)
	{
		if (ImGui::TreeNode(node->getName().c_str()))
		{
			renderNodeContent(node);

			ImGui::TreePop();
		}
	}

	void EngineAssetBrowser::renderResourceNode(std::shared_ptr<Resources::Resource> resource)
	{
		if (ImGui::TreeNode(resource->getName().c_str()))
		{
			ImGui::TreePop();
		}
	}

}
