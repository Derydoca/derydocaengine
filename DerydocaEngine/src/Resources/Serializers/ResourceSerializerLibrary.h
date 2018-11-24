#pragma once
#include <map>
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class ResourceSerializerLibrary
	{
	public:
		static ResourceSerializerLibrary& getInstance()
		{
			static ResourceSerializerLibrary instance;
			return instance;
		}

		ResourceSerializer * getSerializer(ResourceType const& type);

		void operator=(ResourceSerializerLibrary const&) = delete;
	private:
		ResourceSerializerLibrary();
		~ResourceSerializerLibrary();
		ResourceSerializerLibrary(ResourceSerializerLibrary const&);

		void registerSerializer(ResourceSerializer * const& serializer);

		std::map<ResourceType, ResourceSerializer *> m_serializers;
	};

}
