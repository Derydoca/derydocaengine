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
	void registerTypeSerializer(ResourceTypeSerializer* serializer);
	ResourceSerializerLibrary();
	~ResourceSerializerLibrary();
	ResourceSerializerLibrary(ResourceSerializerLibrary const&);

	std::map<ResourceType, ResourceTypeSerializer*> m_typeSerializers;
};

