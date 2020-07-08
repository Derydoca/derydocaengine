#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct SkeletonResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(SkeletonResource);

		SkeletonResource() :
			Resource(ResourceType::SkeletonResourceType),
			m_name()
		{}

		void setName(std::string const& animationName) { m_name = m_name; }
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

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Resources::SkeletonResource, 0);
