#pragma once
//#include "stb_image.h"
#include "SOIL.h"
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <cassert>
#include "Camera.h"

class CubeMap
{
public:
	CubeMap(const std::string& xpos, const std::string& xneg, const std::string& ypos, const std::string& yneg, const std::string& zpos, const std::string& zneg);
	~CubeMap();

	void update(const Camera& camera);
	void bind(unsigned int unit);
private:
	enum {
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint m_texture;
	GLuint m_uniforms[NUM_UNIFORMS];
};

