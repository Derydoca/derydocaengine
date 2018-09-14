#include "BezierCurveRenderer.h"
#include "YamlTools.h"
#include "Shader.h"

namespace DerydocaEngine::Ext
{

	BezierCurveRenderer::BezierCurveRenderer()
	{
	}

	BezierCurveRenderer::~BezierCurveRenderer()
	{
		delete m_material;
	}

	void BezierCurveRenderer::init()
	{
		glGenBuffers(1, &m_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), m_controlPoints, GL_STATIC_DRAW);

		// Setup VBO patch
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		// Set the number of vertices per patch
		glPatchParameteri(GL_PATCH_VERTICES, 4);

		// Segments and strips may be inverted on NVIDIA
		m_material->setInt("NumSegments", m_numSegments);
		m_material->setInt("NumStrips", m_numStrips);
		m_material->setColorRGBA("LineColor", m_lineColor);
	}

	void BezierCurveRenderer::deserialize(YAML::Node const& compNode)
	{
		YAML::Node controlPointsNode = compNode["controlPoints"];
		if (controlPointsNode)
		{
			for (size_t i = 0; i < 8; i++)
			{
				m_controlPoints[i] = controlPointsNode[i].as<float>();
			}
		}

		YAML::Node numSegmentsNode = compNode["numSegments"];
		if (numSegmentsNode)
		{
			m_numSegments = numSegmentsNode.as<int>();
		}

		YAML::Node numStripsNode = compNode["numStrips"];
		if (numStripsNode)
		{
			m_numStrips = numStripsNode.as<int>();
		}

		YAML::Node lineColorNode = compNode["lineColor"];
		if (lineColorNode)
		{
			m_lineColor = lineColorNode.as<Color>();
		}
		else
		{
			m_lineColor = Color(1.0, 1.0, 1.0, 1.0);
		}

		Shader* shader = getResourceObject<Shader>(compNode, "shader");
		m_material = new Rendering::Material();
		m_material->setShader(shader);
	}

	void BezierCurveRenderer::render(MatrixStack * const& matrixStack)
	{
		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		glBindVertexArray(m_vao);
		glDrawArrays(GL_PATCHES, 0, 4);

		glFinish();
	}

}