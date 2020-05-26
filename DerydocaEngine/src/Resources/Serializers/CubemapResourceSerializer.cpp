#include "EnginePch.h"
#include "Resources\Serializers\CubemapResourceSerializer.h"
#include "Resources\CubemapResource.h"
#include "Rendering\Texture.h"
#include "AssetData\CubemapData.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> CubemapResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		try
		{
			AssetData::CubemapData cubemapData;
			{
				std::ifstream fs(resource->getSourceFilePath());
				cereal::JSONInputArchive iarchive(fs);
				iarchive(cubemapData);
			}

			auto cubemapTexture = std::make_shared<Rendering::Texture>(
				ObjectLibrary::getInstance().getResource(cubemapData.texXPos)->getSourceFilePath(),
				ObjectLibrary::getInstance().getResource(cubemapData.texXNeg)->getSourceFilePath(),
				ObjectLibrary::getInstance().getResource(cubemapData.texYPos)->getSourceFilePath(),
				ObjectLibrary::getInstance().getResource(cubemapData.texYNeg)->getSourceFilePath(),
				ObjectLibrary::getInstance().getResource(cubemapData.texZPos)->getSourceFilePath(),
				ObjectLibrary::getInstance().getResource(cubemapData.texZNeg)->getSourceFilePath()
				);
			return cubemapTexture;
		}
		catch (...)
		{
			D_LOG_ERROR("Unable to load cubemap: {}", resource->getSourceFilePath());
		}

		// If we got this far, the data in the source file is not matching what is expected by the resource
		return std::make_shared<Rendering::Texture>();
	}

}
