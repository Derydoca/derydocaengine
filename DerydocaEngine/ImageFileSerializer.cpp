#include "ImageFileSerializer.h"

ImageFileSerializer::ImageFileSerializer()
{
}

ImageFileSerializer::~ImageFileSerializer()
{
}

YAML::Node ImageFileSerializer::generateResourceNodes(std::string filePath)
{
	YAML::Node resources;

	YAML::Node materialResource;
	materialResource["id"] = generateUuid();

	resources.push_back(materialResource);

	return resources;
}

FileType ImageFileSerializer::getFileType()
{
	return FileType::ImageFileType;
}

Resource * ImageFileSerializer::loadResourceFromMeta(YAML::Node resourceNode)
{
	// Create a material resource type
	Resource* r = new Resource();
	r->setType(TextureResourceType);
	return r;
}
