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

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource),
				SERIALIZE(m_name)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource),
				SERIALIZE(m_name)
			);
		}

	private:
		std::string m_name;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Resources::AnimationResource);
