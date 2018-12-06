#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct CubemapResource : public Resource
	{
	public:
		CubemapResource() :
			m_slot(0)
		{}
		~CubemapResource() {}

		void setSlot(unsigned int const& slot) { m_slot = slot; }

		unsigned int getSlot() { return m_slot; }
	private:
		unsigned int m_slot;
	};

}
