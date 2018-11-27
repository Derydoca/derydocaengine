#include "TessellatingQuad.h"

#include <GL\glew.h>
#include <iostream>
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	void TessellatingQuad::init()
	{
		m_material->setInt("Inner", m_inner);
		m_material->setInt("Outer", m_outer);

		glGenBuffers(1, &m_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), m_controlPoints, GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glPatchParameteri(GL_PATCH_VERTICES, 4);

		GLint maxVerts;
		glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxVerts);
		std::cout << "Max patch vertices: " << maxVerts << "\n";
	}

	void TessellatingQuad::deserialize(YAML::Node const& compNode)
	{
		YAML::Node controlPointsNode = compNode["controlPoints"];
		if (controlPointsNode)
		{
			for (size_t i = 0; i < 8; i++)
			{
				m_controlPoints[i] = controlPointsNode[i].as<float>();
			}
		}

		YAML::Node innerNode = compNode["inner"];
		if (innerNode)
		{
			m_inner = innerNode.as<int>();
		}

		YAML::Node outerNode = compNode["outer"];
		if (outerNode)
		{
			m_outer = outerNode.as<int>();
		}

		m_material = getResourceObject<Rendering::Material>(compNode, "material");
	}

	void TessellatingQuad::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		glBindVertexArray(m_vao);
		glDrawArrays(GL_PATCHES, 0, 4);

		glFinish();
	}

}
