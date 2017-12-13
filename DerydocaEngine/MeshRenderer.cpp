#include "MeshRenderer.h"
#include "GameObject.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	m_mesh(mesh),
	m_material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::render(Camera* camera, MatrixStack* matrixStack)
{
	assert(m_material);

	m_material->getShader()->bind();
	glm::mat4 newMatrix = camera->getViewProjection() * matrixStack->getMatrix();
	//m_material->getShader()->update(getGameObject()->getTransform(), camera);
	m_material->getShader()->update(newMatrix);
	m_mesh->draw();
}

void MeshRenderer::update(float deltaTime)
{

}