#include "EnginePch.h"
#include "Resources\Serializers\MaterialResourceSerializer.h"
#include "Rendering\Material.h"
#include "Rendering\Texture.h"
#include "Resources\Resource.h"
#include "Rendering\ShaderLibrary.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<Object> MaterialResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		// Load the yaml file
		YAML::Node root = YAML::LoadFile(resource->getSourceFilePath());

		// Load the shader specified in the file
		boost::uuids::uuid shaderId = root["Shader"].as<boost::uuids::uuid>();
		auto shader = Rendering::ShaderLibrary::getInstance().find(shaderId);

		// Create a material with the shader we created
		auto material = std::make_shared<Rendering::Material>();
		material->setShader(shader);

		// Assign all material parameters to the material
		YAML::Node parameters = root["MaterialParameters"];
		for (size_t i = 0; i < parameters.size(); i++)
		{
			std::string paramType = parameters[i]["Type"].as<std::string>();
			if (paramType == "Texture")
			{
				auto texture = loadResourcePointer<Rendering::Texture>(parameters[i], "ID");

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

}
