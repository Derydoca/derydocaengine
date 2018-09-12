#pragma once

class Mesh;

class Skybox
{
public:
	Skybox();
	Skybox(float const& size);
	~Skybox();

	Mesh* getMesh() const { return m_mesh; };
private:
	void buildMesh(float const& size);

	Mesh* m_mesh;
};

