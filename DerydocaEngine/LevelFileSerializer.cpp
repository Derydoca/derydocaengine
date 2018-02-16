#include "LevelFileSerializer.h"

YAML::Node LevelFileSerializer::generateResourceNodes(std::string filePath)
{
	YAML::Node resources;

	YAML::Node levelResource;
	levelResource["ID"] = generateUuid();

	resources.push_back(levelResource);

	return resources;
}

FileType LevelFileSerializer::getFileType()
{
	return FileType::LevelFileType;
}

Resource * LevelFileSerializer::loadResourceFromMeta(YAML::Node resourceNode)
{
	Resource* r = new Resource();
	r->setType(LevelResourceType);
	return r;
}
