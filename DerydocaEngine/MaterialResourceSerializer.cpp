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

void * MaterialResourceSerializer::deserialize(Resource * const& resource)
{
	// Load the yaml file
	YAML::Node root = YAML::LoadFile(resource->getSourceFilePath());

	// Load the shader specified in the file
	boost::uuids::uuid shaderId = root["Shader"].as<boost::uuids::uuid>();
	DerydocaEngine::Rendering::Shader* shader = DerydocaEngine::Rendering::ShaderLibrary::getInstance().find(shaderId);

	// Create a material with the shader we created
	DerydocaEngine::Rendering::Material* material = new DerydocaEngine::Rendering::Material();
	material->setShader(shader);

	// Assign all material parameters to the material
	YAML::Node parameters = root["MaterialParameters"];
	for (size_t i = 0; i < parameters.size(); i++)
	{
		std::string paramType = parameters[i]["Type"].as<std::string>();
		if (paramType == "Texture")
		{
			auto texture = loadResource<DerydocaEngine::Rendering::Texture*>(parameters[i], "ID");
			YAML::Node nameNode = parameters[i]["Name"];
			if (texture != nullptr && nameNode)
			{
				material->setTexture(nameNode.as<std::string>(), texture);
			}
		}
		else if (paramType == "Color")
		{
			Color paramValue = parameters[i]["Value"].as<Color>();
			std::string paramName = parameters[i]["Name"].as<std::string>();
			material->setColorRGBA(paramName, paramValue);
		}
		else if (paramType == "Float")
		{
			float paramValue = parameters[i]["Value"].as<float>();
			std::string paramName = parameters[i]["Name"].as<std::string>();
			material->setFloat(paramName, paramValue);
		}
		else if (paramType == "Bool")
		{
			bool paramValue = parameters[i]["Value"].as<bool>();
			std::string paramName = parameters[i]["Name"].as<std::string>();
			material->setBool(paramName, paramValue);
		}
	}

	return material;
}

ResourceType MaterialResourceSerializer::getResourceType()
{
	return ResourceType::MaterialResourceType;
}
