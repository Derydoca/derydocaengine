#include "EnginePch.h"
#include "Resources\Serializers\ResourceSerializerLibrary.h"
#include "Resources\Serializers\MaterialResourceSerializer.h"
#include "Resources\Serializers\MeshResourceSerializer.h"
#include "Resources\Serializers\TextureResourceSerializer.h"
#include "Resources\Serializers\CubemapResourceSerializer.h"
#include "Resources\Serializers\ShaderResourceSerializer.h"
#include "Ext\BezierPatchMeshResourceSerializer.h"
#include "Resources\Serializers\SpriteSheetResourceSerializer.h"
#include "Resources\Serializers\AnimationResourceSerializer.h"
#include "Resources\Serializers\SkeletonResourceSerializer.h"
#include "Resources\Serializers\RasterFontResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<ResourceSerializer> ResourceSerializerLibrary::getSerializer(const ResourceType& type)
	{
		// Return the associated serializer for the resource type specified
		auto search = m_serializers.find(type);
		if (search != m_serializers.end())
		{
			return search->second;
		}
		else
		{
			return nullptr;
		}
	}

	ResourceSerializerLibrary::ResourceSerializerLibrary()
	{
		registerSerializer(std::make_shared<AnimationResourceSerializer>());
		registerSerializer(std::make_shared<CubemapResourceSerializer>());
		registerSerializer(std::make_shared<MaterialResourceSerializer>());
		registerSerializer(std::make_shared<MeshResourceSerializer>());
		registerSerializer(std::make_shared<ShaderResourceSerializer>());
		registerSerializer(std::make_shared<SkeletonResourceSerializer>());
		registerSerializer(std::make_shared<SpriteSheetResourceSerializer>());
		registerSerializer(std::make_shared<TextureResourceSerializer>());
		registerSerializer(std::make_shared<Ext::BezierPatchMeshResourceSerializer>());
		registerSerializer(std::make_shared<RasterFontResourceSerializer>());
	}

	ResourceSerializerLibrary::~ResourceSerializerLibrary()
	{
	}

	void ResourceSerializerLibrary::registerSerializer(std::shared_ptr<ResourceSerializer> serializer)
	{
		// Add this serializer to the list of serializers
		m_serializers.insert(std::pair<ResourceType, std::shared_ptr<ResourceSerializer>>(serializer->getResourceType(), serializer));
	}

}
