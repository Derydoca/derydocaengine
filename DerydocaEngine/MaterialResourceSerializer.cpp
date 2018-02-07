#include "MaterialResourceSerializer.h"
#include "Shader.h"
#include "Material.h"
#include "Resource.h"

MaterialResourceSerializer::MaterialResourceSerializer()
{
}

MaterialResourceSerializer::~MaterialResourceSerializer()
{
}

void * MaterialResourceSerializer::deserialize(Resource * resource)
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

ResourceType MaterialResourceSerializer::getResourceType()
{
	return ResourceType::MaterialResourceType;
}
