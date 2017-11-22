#pragma once
#include <glm/glm.hpp>

struct DebugLine
{
public:
	DebugLine();
	~DebugLine();

	inline float getLineWidth() const { return m_lineWidth; }
	inline glm::vec3 getLineStart() const { return m_lineStart; }
	inline glm::vec3 getLineEnd() const { return m_lineEnd; }

	inline void setLineWidth(const float lineWidth) { m_lineWidth = lineWidth; }
	inline void setLineStart(const glm::vec3 lineStart) { m_lineStart = lineStart; }
	inline void setLineEnd(const glm::vec3 lineEnd) { m_lineEnd = lineEnd; }
private:
	float m_lineWidth;
	//Color m_color;
	glm::vec3 m_lineStart;
	glm::vec3 m_lineEnd;
};

