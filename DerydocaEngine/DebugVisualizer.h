#pragma once
#include <glm\glm.hpp>
#include <GL/glew.h>
#include "DebugLine.h"
#include "Shader.h"
#include "GameComponent.h"

class DebugVisualizer : public GameComponent
{
public:
	GENINSTANCE(DebugVisualizer);
	DebugVisualizer();
	~DebugVisualizer();

	void draw(glm::mat4 worldMatrix);

	void postRender();
private:
	void clear();
	
	DebugLine* m_lines;
	int m_lineCount;

	DebugLine m_testLine;

	Shader* m_shader;
};

