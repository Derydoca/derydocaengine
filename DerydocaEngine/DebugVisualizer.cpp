#include "DebugVisualizer.h"

DebugVisualizer::DebugVisualizer()
{
	m_testLine.setLineWidth(2.0f);
	m_testLine.setLineStart(glm::vec3(0, 0, 0));
	m_testLine.setLineEnd(glm::vec3(1, 1, 1));
}


DebugVisualizer::~DebugVisualizer()
{
}

void DebugVisualizer::draw()
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

	glLineWidth(m_testLine.getLineWidth());
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(m_testLine.getLineStart().x, m_testLine.getLineStart().y, m_testLine.getLineStart().z);
	glVertex3f(m_testLine.getLineEnd().x, m_testLine.getLineEnd().y, m_testLine.getLineEnd().z);
	glEnd();
}

void DebugVisualizer::addLine(glm::vec3 start, glm::vec3 end)
{
}

void DebugVisualizer::clear()
{
}
