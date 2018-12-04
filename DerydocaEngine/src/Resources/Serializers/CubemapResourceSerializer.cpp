#include "Resources\Serializers\CubemapResourceSerializer.h"
#include "Resources\CubemapResource.h"
#include "Rendering\Texture.h"

namespace DerydocaEngine::Resources::Serializers
{

	CubemapResourceSerializer::CubemapResourceSerializer()
	{
	}

	CubemapResourceSerializer::~CubemapResourceSerializer()
	{
	}

	std::shared_ptr<void> CubemapResourceSerializer::deserializePointer(Resource * const & resource)
	{
		CubemapResource* cubemapResource = (CubemapResource*)resource;

		// Load the yaml file
		YAML::Node root = YAML::LoadFile(resource->getSourceFilePath());

		YAML::Node materialParamsNode = root["MaterialParameters"];
		if (!materialParamsNode || !materialParamsNode.IsSequence())
		{
			return std::make_shared<Rendering::Texture>();
		}

		for (unsigned int i = 0; i < materialParamsNode.size(); i++)
		{
			YAML::Node parameterNode = materialParamsNode[i];

			// Make sure a slot is defined on this parameter
			YAML::Node slotNode = parameterNode["Slot"];
			if (!slotNode || !slotNode.IsScalar())
			{
				continue;
			}

			// Only continue if the slot matches the one defined on the resource
			if (slotNode.as<int>() != cubemapResource->getSlot())
			{
				continue;
			}

			// Grab all faces of the cubemap and build a cubemap texture from it
			std::string xPosImage = getSourceFilePath(parameterNode, "XPos");
			std::string xNegImage = getSourceFilePath(parameterNode, "XNeg");
			std::string yPosImage = getSourceFilePath(parameterNode, "YPos");
			std::string yNegImage = getSourceFilePath(parameterNode, "YNeg");
			std::string zPosImage = getSourceFilePath(parameterNode, "ZPos");
			std::string zNegImage = getSourceFilePath(parameterNode, "ZNeg");

			// Create the cubemap texture and return the object
			auto cubemapTexture = std::make_shared<Rendering::Texture>(xPosImage, xNegImage, yPosImage, yNegImage, zPosImage, zNegImage);
			return cubemapTexture;
		}

		// If we got this far, the data in the source file is not matching what is expected by the resource
		return std::make_shared<Rendering::Texture>();
	}

	ResourceType CubemapResourceSerializer::getResourceType()
	{
		return CubemapResourceType;
	}

}
