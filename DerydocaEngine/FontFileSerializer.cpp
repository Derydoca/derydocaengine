#include "FontFileSerializer.h"
#include "Resource.h"

FontFileSerializer::FontFileSerializer()
{
}

FontFileSerializer::~FontFileSerializer()
{
}

YAML::Node FontFileSerializer::generateResourceNodes(std::string const& filePath)
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

Resource * FontFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
{
	Resource* r = new Resource();
	r->setType(FontResourceType);
	return r;
}
