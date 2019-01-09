#include "EnginePch.h"
#include "Files\Serializers\RasterFontFileSerializer.h"
#include "Resources\RasterFontResource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node RasterFontFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node levelResource;
		levelResource["ID"] = generateUuid();

		resources.push_back(levelResource);

		return resources;
	}

	Files::FileType RasterFontFileSerializer::getFileType()
	{
		return Files::FileType::RasterFontType;
	}

	std::shared_ptr<Resources::Resource> RasterFontFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		auto r = std::make_shared<Resources::RasterFontResource>();
		return r;
	}

}
