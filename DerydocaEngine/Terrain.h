#pragma once
#include "Mesh.h"
#include "Vertex.h"
#include "SOIL.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "GameComponent.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "Texture.h"

class Terrain : public GameComponent
{
public:
	Terrain(const std::string& fileName, float unitScale, float heightScale);
	Terrain(int width, int depth, float unitScale, float heightScale);
	~Terrain();

	void draw();
	void updateMesh();

	void setTextureSlot(int slot, Texture* texture);

	void init();
	void render(Camera* camera, MatrixStack* matrixStack);
	void update(float deltaTime);
	void postRender();
private:
	int m_width;
	int m_depth;
	float m_unitScale;
	float m_heightScale;
	float** m_heightData;
	Mesh* m_mesh;
	MeshRenderer* m_meshRenderer;
};

