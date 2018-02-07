#include "MaterialFileSerializer.h"
#include <yaml-cpp\yaml.h>
#include "Material.h"
#include "Shader.h"

MaterialSerializer::MaterialSerializer()
{
}

MaterialSerializer::~MaterialSerializer()
{
}

FileType MaterialSerializer::getFileType()
{
	return FileType::MaterialFileType;
}

YAML::Node MaterialSerializer::generateResourceNodes(std::string filePath)
{
	YAML::Node resources;

	YAML::Node materialResource;
	materialResource["id"] = generateUuid();

	resources.push_back(materialResource);

	return resources;
}

Resource * MaterialSerializer::loadResourceFromMeta(YAML::Node resourceNode)
{
	// Create a material resource type
	Resource* r = new Resource();
	r->setType(MaterialResourceType);
	return r;
}
