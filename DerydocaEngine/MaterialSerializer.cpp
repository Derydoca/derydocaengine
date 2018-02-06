#include "MaterialSerializer.h"
#include <yaml-cpp\yaml.h>
#include "Material.h"
#include "Shader.h"

MaterialSerializer::MaterialSerializer()
{
}

MaterialSerializer::~MaterialSerializer()
{
}

ResourceType MaterialSerializer::getResourceType()
{
	return ResourceType::MaterialResourceType;
}

void * MaterialSerializer::deserialize(Resource* resource)
{
	// Load the yaml file
	YAML::Node root = YAML::LoadFile(resource->getSourceFilePath());

	// Load the shader specified in the file
	std::string shaderPath = root["Shader"].as<std::string>();
	Shader* shader = new Shader(shaderPath);

	// Create a material with the shader we created
	Material* material = new Material();
	material->setShader(shader);

	// Assign all material parameters to the material
	YAML::Node parameters = root["MaterialParameters"];
	for (int i = 0; i < parameters.size(); i++)
	{
		std::string paramType = parameters[i]["Type"].as<std::string>();
		if (paramType == "Texture")
		{
			int slot = parameters[i]["Slot"].as<int>();
			std::string texturePath = parameters[i]["Path"].as<std::string>();
			Texture* texture = new Texture(texturePath);
			material->setTextureSlot(slot, texture);
		}
	}

	return material;
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
