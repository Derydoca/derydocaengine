#include "ResourceSerializer.h"
#include "MaterialSerializer.h"

void * ResourceSerializer::loadResource(Resource * resource)
{
	// Find the loader that should be used
	ResourceTypeSerializer* loader = getLoader(resource->getType());

	// If the loader could not be found, return null
	if (loader == nullptr)
	{
		return nullptr;
	}

	// Load the object from the related object loader and return it
	return loader->deserialize(resource);
}

ResourceTypeSerializer * ResourceSerializer::getLoader(ResourceType type)
{
	// Loop through all loaders
	for (ResourceTypeSerializer* l : m_typeLoaders)
	{
		// If the loader's type matches the resource type then return the loader
		if (l->getResourceType() == type)
		{
			return l;
		}
	}

	// Otherwise, return a null loader
	return nullptr;
}

void ResourceSerializer::registerLoader(ResourceTypeSerializer* loader)
{
	m_typeLoaders.push_back(loader);
}

ResourceSerializer::ResourceSerializer()
{
	registerLoader(new MaterialSerializer());
}


ResourceSerializer::~ResourceSerializer()
{
}
