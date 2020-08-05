#include "EngineComponentsPch.h"
#include "TessellatedMeshRenderer.h"

#include <GL\glew.h>
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	TessellatedMeshRenderer::TessellatedMeshRenderer() :
		m_TessellationLevel(4),
		m_UseDynamicTessellation(false),
		m_MinDynamicTessDistance(1.0f),
		m_MaxDynamicTessDistance(10.0f),
		m_MinDynamicTessLevel(2),
		m_MaxDynamicTessLevel(8),
		m_Mesh(),
		m_Material(),
		m_VAO(0),
		m_VBO(0)
	{
	}

	TessellatedMeshRenderer::~TessellatedMeshRenderer()
	{
	}

	void TessellatedMeshRenderer::init()
	{
		auto mesh = m_Mesh.As<Ext::BezierPatchMesh>();

		glGenBuffers(1, &m_VBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH * sizeof(float), mesh->getPatchData(), GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glPatchParameteri(GL_PATCH_VERTICES, 16);

		updateMaterial();
	}

	SERIALIZE_FUNC_LOAD(archive, TessellatedMeshRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_TessellationLevel),
			SERIALIZE(m_UseDynamicTessellation),
			SERIALIZE(m_MinDynamicTessDistance),
			SERIALIZE(m_MaxDynamicTessDistance),
			SERIALIZE(m_MinDynamicTessLevel),
			SERIALIZE(m_MaxDynamicTessLevel),
			SERIALIZE(m_Mesh),
			SERIALIZE(m_Material)
		);

		auto material = std::make_shared<Rendering::Material>();
		material->copyFrom(m_Material.As<Rendering::Material>());
		auto materialResource = std::make_shared<Resources::MaterialResource>();
		materialResource->setData(material);
		m_Material.Set(materialResource);
	}

	SERIALIZE_FUNC_SAVE(archive, TessellatedMeshRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_TessellationLevel),
			SERIALIZE(m_UseDynamicTessellation),
			SERIALIZE(m_MinDynamicTessDistance),
			SERIALIZE(m_MaxDynamicTessDistance),
			SERIALIZE(m_MinDynamicTessLevel),
			SERIALIZE(m_MaxDynamicTessLevel),
			SERIALIZE(m_Mesh),
			SERIALIZE(m_Material)
		);
	}

	void TessellatedMeshRenderer::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		auto material = m_Material.As<Rendering::Material>();
		auto mesh = m_Mesh.As<Ext::BezierPatchMesh>();
		material->bind();
		material->getShader()->updateViaActiveCamera(matrixStack);
		glBindVertexArray(m_VAO);
		glPatchParameteri(GL_PATCH_VERTICES, 16);
		glDrawArrays(GL_PATCHES, 0, mesh->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH);

		glFinish();
	}

	void TessellatedMeshRenderer::renderMesh(
		const std::shared_ptr<Rendering::MatrixStack> matrixStack,
		std::shared_ptr<Rendering::Material> material,
		const Rendering::Projection& projection,
		const std::shared_ptr<Components::Transform> projectionTransform
	)
	{
		// Consider an alternate to using the same material as what is being used to render to screen
		//  The calculations may be excessive for a shadow calc
		material->bind();
		material->getShader()->update(matrixStack, projection, projectionTransform);
		glBindVertexArray(m_VAO);
		glPatchParameteri(GL_PATCH_VERTICES, 16);
		glDrawArrays(GL_PATCHES, 0, m_Mesh.As<Ext::BezierPatchMesh>()->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH);

		glFinish();
	}

	void TessellatedMeshRenderer::update(const float deltaTime)
	{
		updateMaterial();
	}

	void TessellatedMeshRenderer::updateMaterial()
	{
		m_Material.As<Rendering::Material>()->setInt("UseDynamicTessellation", m_UseDynamicTessellation ? 1 : 0);
		m_Material.As<Rendering::Material>()->setInt("TessLevel", m_TessellationLevel);
		m_Material.As<Rendering::Material>()->setFloat("MinTessLevel", (float)m_MinDynamicTessLevel);
		m_Material.As<Rendering::Material>()->setFloat("MaxTessLevel", (float)m_MaxDynamicTessLevel);
		m_Material.As<Rendering::Material>()->setFloat("MinDistance", m_MinDynamicTessDistance);
		m_Material.As<Rendering::Material>()->setFloat("MaxDistance", m_MaxDynamicTessDistance);
	}

}
