#include "EnginePch.h"
#include "ResourceTreeNode.h"

namespace DerydocaEngine::Resources
{
	ResourceTreeNode::ResourceTreeNode(const std::string & name) :
		m_name(name)
	{
	}

	void ResourceTreeNode::addChild(std::weak_ptr<ResourceTreeNode> childNode)
	{
		m_children.push_back(childNode.lock());
	}

	void ResourceTreeNode::addResource(std::weak_ptr<Resource> resource)
	{
		m_resources.push_back(resource.lock());
	}

	void ResourceTreeNode::clear()
	{
		for (auto child : m_children)
		{
			child->clear();
		}

		m_children.clear();
		m_resources.clear();
	}

	std::shared_ptr<ResourceTreeNode> ResourceTreeNode::getChild(const std::string & name) const
	{
		for (auto child : m_children)
		{
			if (child->m_name == name)
			{
				return child;
			}
		}

		return nullptr;
	}

}
