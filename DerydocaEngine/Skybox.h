#pragma once

namespace DerydocaEngine::Rendering {
	class Mesh;
}

class Skybox
{
public:
	Skybox();
	Skybox(float const& size);
	~Skybox();

	DerydocaEngine::Rendering::Mesh* getMesh() const { return m_mesh; };
private:
	void buildMesh(float const& size);

	DerydocaEngine::Rendering::Mesh* m_mesh;
};

