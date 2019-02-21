#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Dgui
{

	std::shared_ptr<Resources::Resource> ResourcePicker(
		const std::string label,
		const std::string& resourceName,
		const Resources::ResourceType resourceType
	);

}