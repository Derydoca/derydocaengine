#pragma once
#include <map>
#include "ResourceSerializer.h"

class ResourceSerializerLibrary
{
public:
	static ResourceSerializerLibrary& getInstance()
	{
		static ResourceSerializerLibrary instance;
		return instance;
	}

	ResourceSerializer * getSerializer(ResourceType type);

	void operator=(ResourceSerializerLibrary const&) = delete;
private:
	ResourceSerializerLibrary();
	~ResourceSerializerLibrary();
	ResourceSerializerLibrary(ResourceSerializerLibrary const&);

	void registerSerializer(ResourceSerializer * serializer);

	std::map<ResourceType, ResourceSerializer *> m_serializers;
};

