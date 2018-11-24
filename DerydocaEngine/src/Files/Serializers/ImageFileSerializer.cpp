#include "Files\Serializers\ImageFileSerializer.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node ImageFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node materialResource;
		materialResource["ID"] = generateUuid();

		resources.push_back(materialResource);

		return resources;
	}

	Files::FileType ImageFileSerializer::getFileType()
	{
		return Files::FileType::ImageFileType;
	}

	Resources::Resource * ImageFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		// Create a material resource type
		Resources::Resource* r = new Resources::Resource();
		r->setType(Resources::TextureResourceType);
		return r;
	}

}
