#include "Files\Serializers\FontFileSerializer.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node FontFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node levelResource;
		levelResource["ID"] = generateUuid();

		resources.push_back(levelResource);

		return resources;
	}

	Files::FileType FontFileSerializer::getFileType()
	{
		return Files::FileType::FontType;
	}

	Resources::Resource * FontFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		Resources::Resource* r = new Resources::Resource();
		r->setType(Resources::FontResourceType);
		return r;
	}

}
