#pragma once
#include <string>
#include <GL/glew.h>
#include <map>
#include "Transform.h"
#include "Camera.h"
#include "Color.h"

class Shader
{
public:
	Shader(const std::string& fileName);
	~Shader();

	void bind();
	void update(const MatrixStack * matrixStack);
	void update(const glm::mat4 matrix);

	void setFloat(std::string name, float val);
	void setVec3(std::string name, glm::vec3 val);
	void setVec4(std::string name, glm::vec4 val);
	void setMat3(std::string name, glm::mat3 val);
	void setMat4(std::string name, glm::mat4 val);
private:
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}
	int getUniformName(std::string stringName);

	enum {
		TRANSFORM_MVP,
		TRANSFORM_MV,
		TRANSFORM_NORMAL,

		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
	std::map<std::string, int> m_uniformLookup;
};

