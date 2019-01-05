#pragma once
#include "Resource.h"

namespace DerydocaEngine::Resources
{
	
	class ResourceTreeNode
	{
	public:
		ResourceTreeNode(const std::string& name);

		void addChild(std::weak_ptr<ResourceTreeNode> childNode);
		void addResource(std::weak_ptr<Resource> resource);
		void clear();
		void sort();

		std::shared_ptr<ResourceTreeNode> getChild(const std::string& name) const;
		std::vector<std::shared_ptr<ResourceTreeNode>> getChildren() const { return m_children; }
		std::string getName() const { return m_name; }
		std::vector<std::shared_ptr<Resource>> getResources() const { return m_resources; }
	private:
		std::string m_name;
		std::vector<std::shared_ptr<ResourceTreeNode>> m_children;
		std::vector<std::shared_ptr<Resource>> m_resources;
	};

}
