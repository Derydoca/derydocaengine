#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct CubemapResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(CubemapResource);

		CubemapResource() :
			m_slot(0)
		{
			setType(Resources::CubemapResourceType);
		}
		~CubemapResource() {}

		void setSlot(unsigned int const& slot) { m_slot = slot; }

		unsigned int getSlot() { return m_slot; }

		SERIALIZE_FUNC_BASE(
			DerydocaEngine::Resources::Resource,
			SERIALIZE(m_slot)
		);

	private:
		unsigned int m_slot;
	};

}
