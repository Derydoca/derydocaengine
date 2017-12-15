#include "MeshRenderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include "CameraManager.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	m_mesh(mesh),
	m_material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::render(MatrixStack* matrixStack)
{
	m_material->bind();
	glm::mat4 newMatrix = CameraManager::getInstance().getCurrentCamera()->getViewProjection() * matrixStack->getMatrix();
	m_material->getShader()->update(newMatrix);
	m_mesh->draw();
}
