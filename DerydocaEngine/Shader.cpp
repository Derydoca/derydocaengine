#include "Shader.h"
#include <fstream>
#include <iostream>
#include "CameraManager.h"

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
	printf("Loading shader: %s\n", fileName.c_str());

	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "VertexPosition");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "VertexNormal");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	m_uniforms[TRANSFORM_MVP] = glGetUniformLocation(m_program, "MVP");
	m_uniforms[TRANSFORM_MV] = glGetUniformLocation(m_program, "ModelViewMatrix");
	m_uniforms[TRANSFORM_NORMAL] = glGetUniformLocation(m_program, "NormalMatrix");
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::update(const MatrixStack * matrixStack)
{
	Camera* camera = CameraManager::getInstance().getCurrentCamera();

	glm::mat4 mvpMatrix = camera->getViewProjectionMatrix() * matrixStack->getMatrix();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_MVP], 1, GL_FALSE, &mvpMatrix[0][0]);

	glm::mat4 mvMatrix = camera->getViewMatrix() * matrixStack->getMatrix();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_MV], 1, GL_FALSE, &mvMatrix[0][0]);

	glm::mat4 normalMatrix = glm::mat3(matrixStack->getMatrix());
	glUniformMatrix3fv(m_uniforms[TRANSFORM_NORMAL], 1, GL_FALSE, &normalMatrix[0][0]);
}

void Shader::update(const glm::mat4 matrix)
{
	glUniformMatrix4fv(m_uniforms[TRANSFORM_MVP], 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setFloat(std::string name, float val)
{
	glUniform1f(getUniformName(name), val);
}

void Shader::setVec3(std::string name, glm::vec3 val)
{
	glUniform3f(getUniformName(name), val.x, val.y, val.z);
}

void Shader::setVec4(std::string name, glm::vec4 val)
{
	glUniform4f(getUniformName(name), val.x, val.y, val.z, val.w);
}

void Shader::setMat3(std::string name, glm::mat3 val)
{
	glUniformMatrix3fv(getUniformName(name), 1, GL_FALSE, &val[0][0]);
}

void Shader::setMat4(std::string name, glm::mat4 val)
{
	glUniformMatrix4fv(getUniformName(name), 1, GL_FALSE, &val[0][0]);
}

int Shader::getUniformName(std::string stringName)
{
	if (m_uniformLookup.count(stringName) != 0)
	{
		return m_uniformLookup[stringName];
	}
	else
	{
		int uniformName = glGetUniformLocation(m_program, stringName.c_str());
		m_uniformLookup[stringName] = uniformName;
		return uniformName;
	}
}

static GLuint CreateShader(const std::string& text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0) {
		std::cerr << "Error: Shader creation failed!" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = static_cast<GLint>(text.length());

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

	return shader;
}

static std::string LoadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram) {
		glGetProgramiv(shader, flag, &success);
	}
	else {
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE) {
		if (isProgram) {
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else {
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}