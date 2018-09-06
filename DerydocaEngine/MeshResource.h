#pragma once
#include "Resource.h"
#include "MeshFlags.h"

struct MeshResource : public Resource
{
public:
	MeshResource() {}

	void setMeshIndex(unsigned int const& meshIndex) { m_meshIndex = meshIndex; }
	void setMeshName(std::string const& meshName) { m_meshName = meshName; }

	unsigned int getMeshIndex() { return m_meshIndex; }
	std::string getMeshName() { return m_meshName; }
	MeshFlags getFlags() { return m_flags; }
	void setFlag(MeshFlags const& flag) {
		m_flags = (MeshFlags)(m_flags | flag);
	}
private:
	unsigned int m_meshIndex;
	std::string m_meshName;
	MeshFlags m_flags{};
};

