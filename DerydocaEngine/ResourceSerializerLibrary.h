#pragma once
#include <map>
#include "ResourceTypeSerializer.h"
#include "ResourceType.h"

class ResourceSerializerLibrary
{
public:
	static ResourceSerializerLibrary& getInstance() {
		static ResourceSerializerLibrary instance;
		return instance;
	}

	ResourceTypeSerializer * getTypeSerializer(std::string sourceFilePath);
	ResourceTypeSerializer * getTypeSerializer(ResourceType type);

	void operator=(ResourceSerializerLibrary const&) = delete;
private:
	ResourceSerializerLibrary();
	~ResourceSerializerLibrary();
	ResourceSerializerLibrary(ResourceSerializerLibrary const&);

	void registerTypeSerializer(ResourceTypeSerializer* serializer);

	std::map<ResourceType, ResourceTypeSerializer*> m_typeSerializers;
};

