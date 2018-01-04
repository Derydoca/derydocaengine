#include "MeshRenderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include "CameraManager.h"
#include "LightManager.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	m_mesh(mesh),
	m_material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::init()
{
}

void MeshRenderer::render(MatrixStack* matrixStack)
{
	assert (getGameObject());

	m_material->bind();
	LightManager::getInstance().bindLightsToShader(getGameObject()->getTransform(), m_material->getShader());
	m_material->getShader()->update(matrixStack);
	m_mesh->draw();
}
