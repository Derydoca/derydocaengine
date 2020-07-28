#pragma once
#include <iostream>
#include <memory>
#include "Resources\ResourceType.h"
#include "ObjectLibrary.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources::Serializers
{

	class ResourceSerializer
	{
	public:
		virtual ~ResourceSerializer() {}
		virtual std::shared_ptr<void> deserializePointer(std::shared_ptr<Resource> resource) { return nullptr; }
		virtual ResourceType getResourceType() = 0;

	};

}
