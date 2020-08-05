#include "EngineComponentsPch.h"
#include "TessellatingQuad.h"

#include <GL\glew.h>
#include <iostream>
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	TessellatingQuad::TessellatingQuad() :
		m_ControlPoints(),
		m_Inner(4),
		m_Outer(4),
		m_Material(),
		m_VAO(0),
		m_VBO(0)
	{
	}

	TessellatingQuad::~TessellatingQuad()
	{
	}

	void TessellatingQuad::init()
	{
		glGenBuffers(1, &m_VBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), m_ControlPoints, GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glPatchParameteri(GL_PATCH_VERTICES, 4);

		GLint maxVerts;
		glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxVerts);
		LOG_INFO("Max patch vertices: {}", maxVerts);

		updateMaterial();
	}

	SERIALIZE_FUNC_LOAD(archive, TessellatingQuad)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_ControlPoints),
			SERIALIZE(m_Inner),
			SERIALIZE(m_Outer),
			SERIALIZE(m_Material)
		);

		auto material = std::make_shared<Rendering::Material>();
		material->copyFrom(m_Material.As<Rendering::Material>());
		auto materialResource = std::make_shared<Resources::MaterialResource>();
		materialResource->setData(material);
		m_Material.Set(materialResource);
	}

	SERIALIZE_FUNC_SAVE(archive, TessellatingQuad)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_ControlPoints),
			SERIALIZE(m_Inner),
			SERIALIZE(m_Outer),
			SERIALIZE(m_Material)
		);
	}

	void TessellatingQuad::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		auto material = m_Material.As<Rendering::Material>();
		material->bind();
		material->getShader()->updateViaActiveCamera(matrixStack);
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_PATCHES, 0, 4);

		glFinish();
	}

	void TessellatingQuad::update(const float deltaTime)
	{
		updateMaterial();
	}

	void TessellatingQuad::updateMaterial()
	{
		auto material = m_Material.As<Rendering::Material>();
		material->setInt("Inner", m_Inner);
		material->setInt("Outer", m_Outer);
	}

}
