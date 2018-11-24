#pragma once
#include "Resources\Serializers\MaterialResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class TextureResourceSerializer : public MaterialResourceSerializer
	{
	public:
		TextureResourceSerializer();
		~TextureResourceSerializer();

		void* deserialize(Resource* const& resource);
		ResourceType getResourceType();
	};

}
