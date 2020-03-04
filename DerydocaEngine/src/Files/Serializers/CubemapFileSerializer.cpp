#include "EnginePch.h"
#include "Files\Serializers\CubemapFileSerializer.h"
#include <yaml-cpp\yaml.h>
#include "Rendering\Material.h"
#include "Rendering\Shader.h"
#include "Resources\CubemapResource.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Files::Serializers {

	Files::FileType CubemapSerializer::getFileType()
	{
		return Files::FileType::CubemapFileType;
	}

	YAML::Node CubemapSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;
		D_LOG_ERROR("Deprecated code that will likely cause errors has been triggered.");
		return resources;
	}

	std::vector<std::shared_ptr<Resources::Resource>> CubemapSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();

		auto resource = std::make_shared<Resources::CubemapResource>();
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

					// If it is a cubemap, then create a cubemap resource
					if (parameterType == "Cubemap")
					{
						resource->setSlot(i);
					}
				}
			}
		}

		resources.push_back(resource);

		return resources;
	}

}
