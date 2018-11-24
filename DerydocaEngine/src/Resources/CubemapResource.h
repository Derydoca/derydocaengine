#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct CubemapResource : public Resource
	{
	public:
		CubemapResource() {}
		~CubemapResource() {}

		void setSlot(unsigned int const& slot) { m_slot = slot; }

		unsigned int getSlot() { return m_slot; }
	private:
		unsigned int m_slot;
	};

}
