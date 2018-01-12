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
	void setColorRGB(std::string name, Color color);
	void setColorRGBA(std::string name, Color color);
	void setInt(std::string name, int val);
	void setVec3(std::string name, glm::vec3 val);
	void setVec4(std::string name, glm::vec4 val);
	void setMat3(std::string name, glm::mat3 val);
	void setMat4(std::string name, glm::mat4 val);

	std::string GetLoadPath() const { return m_loadPath; }
	std::string GetVertexShaderPath() const { return m_loadPath + ".vs"; }
	std::string GetFragmentShaderPath() const { return m_loadPath + ".fs"; }
private:
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}
	int getUniformName(std::string stringName);

	enum {
		TRANSFORM_MVP = 0,
		TRANSFORM_MV = 1,
		TRANSFORM_NORMAL = 2,
		TRANSFORM_PROJECTION = 3,
		TRANSFORM_MODEL = 4,

		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLint m_uniforms[NUM_UNIFORMS];
	std::string m_loadPath;
	std::map<std::string, int> m_uniformLookup;
};

