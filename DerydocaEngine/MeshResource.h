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
	DerydocaEngine::Rendering::MeshFlags getFlags() { return m_flags; }
	void setFlag(DerydocaEngine::Rendering::MeshFlags const& flag) {
		m_flags = (DerydocaEngine::Rendering::MeshFlags)(m_flags | flag);
	}
private:
	unsigned int m_meshIndex;
	std::string m_meshName;
	DerydocaEngine::Rendering::MeshFlags m_flags{};
};

