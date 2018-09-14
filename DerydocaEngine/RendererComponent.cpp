#include "RendererComponent.h"
#include "LightManager.h"

namespace DerydocaEngine::Components
{

	void RendererComponent::updateMesh()
	{
		if (m_dirtyComponents == Rendering::MeshComponents::None)
		{
			return;
		}

		if (!m_mesh)
		{
			m_mesh = new Rendering::Mesh();
		}

		if (m_dirtyComponents & Rendering::MeshComponents::Positions)
		{
			m_vertices = generateVertices();
			m_numVertices = generateNumVertices();
		}
		if (m_dirtyComponents & Rendering::MeshComponents::TexCoords)
		{
			m_texCoords = generateTexCoords();
		}
		if (m_dirtyComponents & Rendering::MeshComponents::Colors)
		{
			m_vertexColors = generateVertexColors();
		}
		if (m_dirtyComponents & Rendering::MeshComponents::Indices)
		{
			m_triangleIndices = generateTriangleIndices();
			m_numIndices = generateNumIndices();
		}
		if (m_dirtyComponents & Rendering::MeshComponents::Tangents)
		{
			m_tangents = generateTangents();
		}
		if (m_dirtyComponents & Rendering::MeshComponents::Bitangents)
		{
			m_bitangents = generateBitangents();
		}
		if (m_dirtyComponents & Rendering::MeshComponents::Normals)
		{
			m_normals = generateNormals();
		}

		m_mesh->loadMeshComponentData(
			m_dirtyComponents,
			m_numVertices,
			m_vertices,
			m_tangents,
			m_bitangents,
			m_texCoords,
			m_normals,
			m_numIndices,
			m_triangleIndices,
			m_vertexColors
		);

		m_dirtyComponents = Rendering::MeshComponents::None;
	}

	void RendererComponent::render(Rendering::MatrixStack * const& matrixStack)
	{
		if (m_dirtyComponents != Rendering::MeshComponents::None)
		{
			updateMesh();
		}

		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		Rendering::LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

		m_mesh->draw();

		m_material->unbind();
	}

	void RendererComponent::renderMesh(Rendering::MatrixStack * const& matrixStack, Rendering::Material * const& material, Projection const& projection, Transform * const& projectionTransform)
	{
		m_material->bind();
		m_material->getShader()->update(matrixStack, projection, projectionTransform);
		m_mesh->draw();
		m_material->unbind();
	}

}
