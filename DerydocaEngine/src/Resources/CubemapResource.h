#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct CubemapResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(CubemapResource);

		CubemapResource() :
			Resource(ResourceType::CubemapResourceType),
			m_slot(0)
		{}
		~CubemapResource() {}

		void setSlot(unsigned int const& slot) { m_slot = slot; }

		unsigned int getSlot() { return m_slot; }

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource),
				SERIALIZE(m_slot)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource),
				SERIALIZE(m_slot)
			);
		}

	private:
		unsigned int m_slot;
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::CubemapResource);
