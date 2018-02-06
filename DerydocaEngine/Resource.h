#pragma once
#include <string>
#include "ResourceType.h"
#include <boost/uuid/uuid.hpp>

using namespace boost::uuids;

struct Resource
{
public:
	Resource();
	Resource(uuid id, std::string sourceFilePath, std::string metaFilePath);
	Resource(uuid id, std::string sourceFilePath, std::string metaFilePath, ResourceType type);

	void setId(uuid id) { m_id = id; }
	void setFilePaths(std::string sourceFilePath, std::string metaFilePath) { m_sourceFilePath = sourceFilePath; m_metaFilePath = metaFilePath; }
	void setType(ResourceType type) { m_type = type; }

	uuid getId() { return m_id; }
	ResourceType getType() { return m_type; }
	std::string getSourceFilePath() { return m_sourceFilePath; }
	std::string getMetaFilePath() { return m_metaFilePath; }
	void* getResourceObject();
private:
	uuid m_id;
	ResourceType m_type;
	std::string m_sourceFilePath;
	std::string m_metaFilePath;
	void* m_resourceObject = 0;
};
