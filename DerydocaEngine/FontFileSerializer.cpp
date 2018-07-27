#include "FontFileSerializer.h"

FontFileSerializer::FontFileSerializer()
{
}

FontFileSerializer::~FontFileSerializer()
{
}

YAML::Node FontFileSerializer::generateResourceNodes(std::string filePath)
{
	YAML::Node resources;

	YAML::Node levelResource;
	levelResource["ID"] = generateUuid();

	resources.push_back(levelResource);

	return resources;
}

FileType FontFileSerializer::getFileType()
{
	return FileType::FontType;
}

Resource * FontFileSerializer::loadResourceFromMeta(YAML::Node resourceNode)
{
	Resource* r = new Resource();
	r->setType(FontResourceType);
	return r;
}
