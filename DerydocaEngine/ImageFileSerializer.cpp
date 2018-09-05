#include "ImageFileSerializer.h"

ImageFileSerializer::ImageFileSerializer()
{
}

ImageFileSerializer::~ImageFileSerializer()
{
}

YAML::Node ImageFileSerializer::generateResourceNodes(std::string const& filePath)
{
	YAML::Node resources;

	YAML::Node materialResource;
	materialResource["ID"] = generateUuid();

	resources.push_back(materialResource);

	return resources;
}

FileType ImageFileSerializer::getFileType()
{
	return FileType::ImageFileType;
}

Resource * ImageFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
{
	// Create a material resource type
	Resource* r = new Resource();
	r->setType(TextureResourceType);
	return r;
}
