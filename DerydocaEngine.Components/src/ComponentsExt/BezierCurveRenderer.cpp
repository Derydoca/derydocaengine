#include "EngineComponentsPch.h"
#include "BezierCurveRenderer.h"

#include <GL/glew.h>
#include "Helpers\YamlTools.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	BezierCurveRenderer::BezierCurveRenderer()
	{
	}

	BezierCurveRenderer::~BezierCurveRenderer()
	{
	}

	void BezierCurveRenderer::init()
	{
		glGenBuffers(1, &m_VBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), m_ControlPoints, GL_STATIC_DRAW);

		// Setup VBO patch
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		// Set the number of vertices per patch
		glPatchParameteri(GL_PATCH_VERTICES, 4);
	}

	SERIALIZE_FUNC_LOAD(archive, BezierCurveRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_ControlPoints),
			SERIALIZE(m_NumSegments),
			SERIALIZE(m_NumStrips),
			SERIALIZE(m_LineColor),
			SERIALIZE(m_Shader)
		);

		m_Material = std::make_shared<Rendering::Material>();
		m_Material->setShader(m_Shader);
	}

	SERIALIZE_FUNC_SAVE(archive, BezierCurveRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_ControlPoints),
			SERIALIZE(m_NumSegments),
			SERIALIZE(m_NumStrips),
			SERIALIZE(m_LineColor),
			SERIALIZE(m_Shader)
		);
	}

	void BezierCurveRenderer::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		// Segments and strips may be inverted on NVIDIA
		m_Material->setInt("NumSegments", m_NumSegments);
		m_Material->setInt("NumStrips", m_NumStrips);
		m_Material->setColorRGBA("LineColor", m_LineColor);

		m_Material->bind();
		m_Material->getShader()->updateViaActiveCamera(matrixStack);
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_PATCHES, 0, 4);

		glFinish();
	}

}