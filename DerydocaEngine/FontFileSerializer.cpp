#include "FontFileSerializer.h"
#include "Resource.h"

namespace DerydocaEngine::FileSerializers {

	YAML::Node FontFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node levelResource;
		levelResource["ID"] = generateUuid();

		resources.push_back(levelResource);

		return resources;
	}

	FileType FontFileSerializer::getFileType()
	{
		return FileType::FontType;
	}

	Resources::Resource * FontFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		Resources::Resource* r = new Resources::Resource();
		r->setType(Resources::FontResourceType);
		return r;
	}

}
