#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Transform.h"

class MeshRenderer : public GameComponent
{
public:
	MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();
	void render(MatrixStack* matrixStack);
	Material* getMaterial() { return m_material; }

	void init();
private:
	Mesh* m_mesh;
	Material* m_material;
};
