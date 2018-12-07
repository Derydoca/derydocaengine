#include "RendererComponent.h"

#include "Rendering\LightManager.h"
#include "Rendering\Material.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Components
{

	RendererComponent::RendererComponent() :
		m_mesh(),
		m_material(),
		m_dirtyComponents(),
		m_vertices(),
		m_tangents(),
		m_bitangents(),
		m_normals(),
		m_texCoords(),
		m_vertexColors(),
		m_triangleIndices()
	{
	}

	RendererComponent::~RendererComponent()
	{
	}

	void RendererComponent::updateMesh()
	{
		if (m_dirtyComponents == Rendering::MeshComponents::None)
		{
			return;
		}

		if (!m_mesh)
		{
			m_mesh = std::make_unique<Rendering::Mesh>();
		}

		if (m_dirtyComponents & Rendering::MeshComponents::Positions)
		{
			m_vertices = generateVertices();
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
			m_vertices,
			m_triangleIndices,
			m_normals,
			m_texCoords,
			m_tangents,
			m_bitangents,
			m_vertexColors
		);

		m_dirtyComponents = Rendering::MeshComponents::None;
	}

	void RendererComponent::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
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

	void RendererComponent::renderMesh(
		const std::shared_ptr<Rendering::MatrixStack> matrixStack,
		std::shared_ptr<Rendering::Material> material,
		const Rendering::Projection& projection,
		const std::shared_ptr<Transform> projectionTransform
	)
	{
		m_material->bind();
		m_material->getShader()->update(matrixStack, projection, projectionTransform);
		m_mesh->draw();
		m_material->unbind();
	}

}
