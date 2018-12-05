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

		std::shared_ptr<ResourceSerializer> getSerializer(const ResourceType& type);
	private:
		ResourceSerializerLibrary();
		~ResourceSerializerLibrary();

		ResourceSerializerLibrary(const ResourceSerializerLibrary&);
		void operator=(const ResourceSerializerLibrary&) = delete;
		void registerSerializer(std::shared_ptr<ResourceSerializer> serializer);

		std::map<ResourceType, std::shared_ptr<ResourceSerializer>> m_serializers;
	};

}
