#pragma once
#include <glm/glm.hpp>

class DebugLine
{
public:
	DebugLine();
	~DebugLine();
private:
	float m_lineWidth;
	//Color m_color;
	glm::vec3 m_lineStart;
	glm::vec3 m_lineEnd;
	/*
	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(15, 0, 0);
	glEnd();
	*/
};

