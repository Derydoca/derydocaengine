#pragma once
#include <string>

struct Resource
{
public:
	Resource(int id, std::string sourceFilePath, std::string metaFilePath) :
		m_id(id),
		m_sourceFilePath(sourceFilePath),
		m_metaFilePath(metaFilePath) {}

	int getId() { return m_id; }
	std::string getSourceFilePath() { return m_sourceFilePath; }
	std::string getMetaFilePath() { return m_metaFilePath; }
private:
	int m_id;
	std::string m_sourceFilePath;
	std::string m_metaFilePath;
};
