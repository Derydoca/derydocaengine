#include "TessellatedMeshRenderer.h"
#include "Shader.h"

TessellatedMeshRenderer::TessellatedMeshRenderer()
{
}

TessellatedMeshRenderer::~TessellatedMeshRenderer()
{
}

void TessellatedMeshRenderer::init()
{
	m_material->setInt("TessLevel", m_tessellationLevel);

	glGenBuffers(1, &m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_mesh->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH * sizeof(float), m_mesh->getPatchData(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glPatchParameteri(GL_PATCH_VERTICES, 16);
}

void TessellatedMeshRenderer::deserialize(YAML::Node compNode)
{
	YAML::Node tessellationLevelNode = compNode["tessellationLevel"];
	if (tessellationLevelNode)
	{
		m_tessellationLevel = tessellationLevelNode.as<int>();
	}

	m_material = loadResource<Material*>(compNode, "material");
	m_mesh = loadResource<BezierPatchMesh*>(compNode, "bezierPatchMesh");
}

void TessellatedMeshRenderer::render(MatrixStack * matrixStack)
{
	m_material->bind();
	m_material->getShader()->update(matrixStack);
	glBindVertexArray(m_vao);
	glPatchParameteri(GL_PATCH_VERTICES, 16);
	glDrawArrays(GL_PATCHES, 0, m_mesh->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH);

	glFinish();
}
