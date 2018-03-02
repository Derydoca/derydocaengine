#include "MaterialResourceSerializer.h"
#include "Material.h"
#include "Resource.h"
#include "ShaderLibrary.h"

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
	uuid shaderId = root["Shader"].as<uuid>();
	Shader* shader = ShaderLibrary::getInstance().find(shaderId);

	// Create a material with the shader we created
	Material* material = new Material();
	material->setShader(shader);

	// Assign all material parameters to the material
	YAML::Node parameters = root["MaterialParameters"];
	for (size_t i = 0; i < parameters.size(); i++)
	{
		std::string paramType = parameters[i]["Type"].as<std::string>();
		if (paramType == "Texture")
		{
			int slot = parameters[i]["Slot"].as<int>();
			auto texture = loadResource<Texture*>(parameters[i], "ID");
			material->setTextureSlot(slot, texture);
		}
		if (paramType == "Color")
		{
			Color paramColor = parameters[i]["Value"].as<Color>();
			std::string paramName = parameters[i]["Name"].as<string>();
			material->setColorRGBA(paramName, paramColor);
		}
	}

	return material;
}

ResourceType MaterialResourceSerializer::getResourceType()
{
	return ResourceType::MaterialResourceType;
}
