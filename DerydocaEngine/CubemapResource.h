#pragma once
#include "Resource.h"

struct CubemapResource : public Resource
{
public:
	CubemapResource() {}
	~CubemapResource() {}

	void setSlot(unsigned int slot) { m_slot = slot; }

	unsigned int getSlot() { return m_slot; }
private:
	unsigned int m_slot;
};