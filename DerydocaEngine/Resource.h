#pragma once
#include <string>
#include "ResourceType.h"
#include "ResourceSerializer.h"

struct Resource
{
public:
	Resource(int id, std::string sourceFilePath, std::string metaFilePath);

	int getId() { return m_id; }
	ResourceType getType() { return m_type; }
	std::string getSourceFilePath() { return m_sourceFilePath; }
	std::string getMetaFilePath() { return m_metaFilePath; }
	void* getResourceObject();
private:
	int m_id;
	ResourceType m_type;
	std::string m_sourceFilePath;
	std::string m_metaFilePath;
	void* m_resourceObject = 0;
};
