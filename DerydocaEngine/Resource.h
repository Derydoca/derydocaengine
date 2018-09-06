#pragma once
#include <string>
#include "ResourceType.h"
#include <boost/uuid/uuid.hpp>

using namespace boost::uuids;

struct Resource
{
public:
	Resource();
	Resource(uuid const& id, std::string const& sourceFilePath, std::string const& metaFilePath, ResourceType const& type);

	void setId(uuid const& id) { m_id = id; }
	void setFilePaths(std::string const& sourceFilePath, std::string const& metaFilePath) { m_sourceFilePath = sourceFilePath; m_metaFilePath = metaFilePath; }
	void setType(ResourceType const& type) { m_type = type; }

	uuid getId() const { return m_id; }
	ResourceType getType() const { return m_type; }
	std::string getSourceFilePath() const { return m_sourceFilePath; }
	std::string getMetaFilePath() const { return m_metaFilePath; }
	void* getResourceObject();
protected:
	uuid m_id;
	ResourceType m_type;
	std::string m_sourceFilePath;
	std::string m_metaFilePath;
	void* m_resourceObject = 0;
};
