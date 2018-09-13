#include "DebugVisualizer.h"
#include "CameraManager.h"
#include "ShaderLibrary.h"

namespace DerydocaEngine::Debug {

	DebugVisualizer::DebugVisualizer()
	{
		m_shader = ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\debugVisShader");

		m_testLine.setLineWidth(2.0f);
		m_testLine.setLineStart(glm::vec3(0, 0, 0));
		m_testLine.setLineEnd(glm::vec3(1, 1, 1));
	}

	DebugVisualizer::~DebugVisualizer()
	{
		delete(m_shader);
	}

	void DebugVisualizer::draw(glm::mat4 const& worldMatrix)
	{
		/*for (int i = 0; i < m_lineCount; i++)
		{
			DebugLine line = m_lines[i];

			glLineWidth(line.GetLineWidth());
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(line.GetLineStart().x, line.GetLineStart().y, line.GetLineStart().z);
			glVertex3f(line.GetLineEnd().x, line.GetLineEnd().y, line.GetLineEnd().z);
			glEnd();
		}*/

		m_shader->bind();
		m_shader->update(worldMatrix);

		glLineWidth(m_testLine.getLineWidth());
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(m_testLine.getLineStart().x, m_testLine.getLineStart().y, m_testLine.getLineStart().z);
		glVertex3f(m_testLine.getLineEnd().x, m_testLine.getLineEnd().y, m_testLine.getLineEnd().z);
		glEnd();
	}

	void DebugVisualizer::postRender()
	{
		DerydocaEngine::Components::Camera* currentCamera = CameraManager::getInstance().getCurrentCamera();
		if (currentCamera == nullptr)
		{
			return;
		}
		glm::mat4 model = currentCamera->getGameObject()->getTransform()->getModel();
		draw(currentCamera->getProjection().getInverseViewProjectionMatrix(model));
	}

	void DebugVisualizer::clear()
	{
	}

}
