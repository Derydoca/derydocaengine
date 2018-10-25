#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include "ResourceType.h"

namespace DerydocaEngine::Resources
{

	struct Resource
	{
	public:
		Resource();
		Resource(boost::uuids::uuid const& id, std::string const& sourceFilePath, std::string const& metaFilePath, ResourceType const& type);

		void setId(boost::uuids::uuid const& id) { m_id = id; }
		void setFilePaths(std::string const& sourceFilePath, std::string const& metaFilePath) { m_sourceFilePath = sourceFilePath; m_metaFilePath = metaFilePath; }
		void setType(ResourceType const& type) { m_type = type; }

		boost::uuids::uuid getId() const { return m_id; }
		ResourceType getType() const { return m_type; }
		std::string getSourceFilePath() const { return m_sourceFilePath; }
		std::string getMetaFilePath() const { return m_metaFilePath; }
		void* getResourceObject();
		std::shared_ptr<void> getResourceObjectPointer();
	protected:
		boost::uuids::uuid m_id;
		ResourceType m_type;
		std::string m_sourceFilePath;
		std::string m_metaFilePath;
		void* m_resourceObject = 0;
		std::shared_ptr<void> m_resourceObjectPointer;
	};

}
