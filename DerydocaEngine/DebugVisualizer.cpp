#include "DebugVisualizer.h"

DebugVisualizer::DebugVisualizer()
{
	m_testLine.SetLineWidth(2.0f);
	m_testLine.SetLineStart(glm::vec3(0, 0, 0));
	m_testLine.SetLineEnd(glm::vec3(1, 1, 1));
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

	glLineWidth(m_testLine.GetLineWidth());
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(m_testLine.GetLineStart().x, m_testLine.GetLineStart().y, m_testLine.GetLineStart().z);
	glVertex3f(m_testLine.GetLineEnd().x, m_testLine.GetLineEnd().y, m_testLine.GetLineEnd().z);
	glEnd();
}

void DebugVisualizer::addLine(glm::vec3 start, glm::vec3 end)
{
}

void DebugVisualizer::clear()
{
}
