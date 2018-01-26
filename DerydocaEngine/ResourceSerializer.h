#pragma once
#include <vector>

enum ResourceType;
class Resource;
class ResourceTypeSerializer;

class ResourceSerializer
{
public:
	static ResourceSerializer& getInstance()
	{
		static ResourceSerializer instance;
		return instance;
	}
	void operator=(ResourceSerializer const&) = delete;

	void* loadResource(Resource * resource);
private:
	std::vector<ResourceTypeSerializer*> m_typeLoaders;
	ResourceTypeSerializer * getLoader(enum ResourceType type);

	void registerLoader(ResourceTypeSerializer* loader);
	ResourceSerializer();
	~ResourceSerializer();
};

