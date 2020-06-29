#include "EnginePch.h"
#include "RasterFontResourceSerializer.h"
#include "UI\FontFace.h"

std::shared_ptr<void> DerydocaEngine::Resources::Serializers::RasterFontResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
{
	std::shared_ptr<UI::FontFace> ff = std::make_shared<UI::FontFace>();
	ff->loadFromSerializedFile(resource->getSourceFilePath());
	return ff;
}
