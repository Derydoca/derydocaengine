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

void MeshRenderer::init()
{
}

void MeshRenderer::render(MatrixStack* matrixStack)
{
	m_material->bind();
	m_material->getShader()->update(matrixStack);
	m_mesh->draw();
}
