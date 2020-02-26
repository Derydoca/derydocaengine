#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct AnimationResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(AnimationResource);

		AnimationResource() :
			Resource(ResourceType::AnimationResourceType),
			m_name()
		{}

		void setName(std::string const& animationName) { m_name = animationName; }
		std::string getName() { return m_name; }

		SERIALIZE_FUNC_BASE(
			DerydocaEngine::Resources::Resource,
			SERIALIZE(m_name)
		);
	private:
		std::string m_name;
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::AnimationResource);
