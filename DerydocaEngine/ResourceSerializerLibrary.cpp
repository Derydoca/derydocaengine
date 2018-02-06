#include "ResourceSerializerLibrary.h"

#include "MaterialSerializer.h"
#include "MeshSerializer.h"

ResourceTypeSerializer * ResourceSerializerLibrary::getTypeSerializer(std::string sourceFilePath)
{
	ResourceType type = pathToResourceType(sourceFilePath);
	return getTypeSerializer(type);
}

ResourceTypeSerializer * ResourceSerializerLibrary::getTypeSerializer(ResourceType type)
{
	auto search = m_typeSerializers.find(type);
	if (search != m_typeSerializers.end())
	{
		return search->second;
	}
	else
	{
		return nullptr;
	}
}

void ResourceSerializerLibrary::registerTypeSerializer(ResourceTypeSerializer * serializer)
{
	m_typeSerializers.insert(std::pair<ResourceType, ResourceTypeSerializer*>(serializer->getResourceType(), serializer));
}

ResourceSerializerLibrary::ResourceSerializerLibrary()
{
	// Register all type serializers
	registerTypeSerializer(new MaterialSerializer());
	registerTypeSerializer(new MeshSerializer());
}

ResourceSerializerLibrary::~ResourceSerializerLibrary()
{
	// Delete all type serializers
	for (std::map<ResourceType, ResourceTypeSerializer*>::iterator it = m_typeSerializers.begin(); it != m_typeSerializers.end(); ++it)
	{
		delete(it->second);
	}
}
