#pragma once
#include <glm/glm.hpp>

struct DebugLine
{
public:
	DebugLine();
	~DebugLine();

	inline float GetLineWidth() const { return m_lineWidth; }
	inline glm::vec3 GetLineStart() const { return m_lineStart; }
	inline glm::vec3 GetLineEnd() const { return m_lineEnd; }

	inline void SetLineWidth(const float lineWidth) { m_lineWidth = lineWidth; }
	inline void SetLineStart(const glm::vec3 lineStart) { m_lineStart = lineStart; }
	inline void SetLineEnd(const glm::vec3 lineEnd) { m_lineEnd = lineEnd; }
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

