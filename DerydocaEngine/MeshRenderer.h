#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Transform.h"

class MeshRenderer : public GameComponent
{
public:
	GENINSTANCE(MeshRenderer)
	MeshRenderer();
	MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();
	void render(MatrixStack* matrixStack);
	Material* getMaterial() { return m_material; }

	bool deserialize(YAML::Node compNode);

	void init();
private:
	Mesh* m_mesh;
	Material* m_material;

	void setMesh(Mesh* mesh) { m_mesh = mesh; }
	void setMaterial(Material* material) { m_material = material; }
};
