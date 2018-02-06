#include "ResourceSerializerLibrary.h"

#include "MaterialSerializer.h"
#include "MeshSerializer.h"

ResourceTypeSerializer * ResourceSerializerLibrary::getTypeSerializer(std::string sourceFilePath)
{
	// Determine the resource type from the file path
	ResourceType type = pathToResourceType(sourceFilePath);

	// Return the serializer associated to that type
	return getTypeSerializer(type);
}

ResourceTypeSerializer * ResourceSerializerLibrary::getTypeSerializer(ResourceType type)
{
	// Return the associated serializer for the resource type specified
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
	// Add this serializer to the list of serializers
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
