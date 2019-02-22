#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Dgui
{

	bool ResourcePicker(
		const std::string label,
		std::shared_ptr<Resources::Resource> resource,
		std::shared_ptr<Resources::Resource>& selectedResource
	);

}