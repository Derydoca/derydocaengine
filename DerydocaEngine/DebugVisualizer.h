#pragma once
#include <glm\glm.hpp>
#include <GL/glew.h>

class DebugVisualizer
{
public:
	DebugVisualizer();
	~DebugVisualizer();

	void draw();

	void addLine(glm::vec3 start, glm::vec3 end);
private:
	void clear();
};

