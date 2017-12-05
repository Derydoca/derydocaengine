#pragma once
#include <glm\glm.hpp>
#include <GL/glew.h>
#include "DebugLine.h"
#include "Shader.h"

class DebugVisualizer
{
public:
	DebugVisualizer();
	~DebugVisualizer();

	void draw(glm::mat4 worldMatrix);

	void addLine(glm::vec3 start, glm::vec3 end);
private:
	void clear();
	
	DebugLine* m_lines;
	int m_lineCount;

	DebugLine m_testLine;

	Shader* m_shader;
};

