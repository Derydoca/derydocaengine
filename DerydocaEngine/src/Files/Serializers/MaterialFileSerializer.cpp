#include "EnginePch.h"
#include "Files\Serializers\MaterialFileSerializer.h"
#include <yaml-cpp\yaml.h>
#include "Rendering\Material.h"
#include "Rendering\Shader.h"
#include "Resources\CubemapResource.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Files::Serializers {

	Files::FileType MaterialSerializer::getFileType()
	{
		return Files::FileType::MaterialFileType;
	}

	YAML::Node MaterialSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node materialResource;
		materialResource["Type"] = "Material";
		materialResource["ID"] = generateUuid();

		resources.push_back(materialResource);

		return resources;
	}

	std::vector<std::shared_ptr<Resources::Resource>> MaterialSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();

		auto resource = std::make_shared<Resources::MaterialResource>();
		resource->generateAndSetId();
		// Set static type value somehow...

		{ // TODO: Remove YAML parsing when materials have been converted to use Cereal
			// Load the yaml file
			YAML::Node root = YAML::LoadFile(assetPath);

			// Open the material parameters node
			YAML::Node materialParamNode = root["MaterialParameters"];
			if (materialParamNode && materialParamNode.IsSequence())
			{
				// Iterate through each parameter
				for (unsigned int i = 0; i < materialParamNode.size(); i++)
				{
					// Get the type of the parameter
					YAML::Node parameterTypeNode = materialParamNode[i]["Type"];
					if (!parameterTypeNode || !parameterTypeNode.IsScalar())
					{
						continue;
					}
					std::string parameterType = parameterTypeNode.as<std::string>();
				}
			}
		}

		resources.push_back(resource);

		return resources;
	}

}
