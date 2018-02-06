#pragma once
#include "Resource.h"
struct MeshResource : public Resource
{
public:
	MeshResource();

	void setMeshIndex(unsigned int meshIndex) { m_meshIndex = meshIndex; }
	void setMeshName(std::string meshName) { m_meshName = meshName; }

	unsigned int getMeshIndex() { return m_meshIndex; }
	std::string getMeshName() { return m_meshName; }
private:
	unsigned int m_meshIndex;
	std::string m_meshName;
};

