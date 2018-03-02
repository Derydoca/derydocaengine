#include "Shader.h"
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "CameraManager.h"
#include "GLError.h"

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
	m_loadPath = fileName;

	printf("Loading shader: %s\n", fileName.c_str());

	// Create the shaders
	m_shaders[0] = CreateShader(LoadShader(GetVertexShaderPath()), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(GetFragmentShaderPath()), GL_FRAGMENT_SHADER);

	// Create the program
	m_program = glCreateProgram();
	if (0 == m_program)
	{
		fprintf(stderr, "Error creating program object..\n");
		__debugbreak();
	}

	// Attach the shaders to the program
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(m_program, m_shaders[i]);
	}

	// Get the vertex attribute locations
	glBindAttribLocation(m_program, 0, "VertexPosition");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "VertexNormal");

	// Bind the output color to 0
	glBindFragDataLocation(m_program, 0, "FragColor");

	// Link the program
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	m_uniforms[TRANSFORM_MVP] = glGetUniformLocation(m_program, "MVP");
	m_uniforms[TRANSFORM_MV] = glGetUniformLocation(m_program, "ModelViewMatrix");
	m_uniforms[TRANSFORM_NORMAL] = glGetUniformLocation(m_program, "NormalMatrix");
	m_uniforms[TRANSFORM_PROJECTION] = glGetUniformLocation(m_program, "ProjectionMatrix");
	m_uniforms[TRANSFORM_MODEL] = glGetUniformLocation(m_program, "ModelMatrix");
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

void printMatrix(std::string matName, glm::mat4 mat)
{
	auto matPtr = glm::value_ptr(mat);
	printf("\n%s:\n", matName.c_str());
	printf("    %f   %f   %f   %f\n", matPtr[0], matPtr[1], matPtr[2], matPtr[3]);
	printf("    %f   %f   %f   %f\n", matPtr[4], matPtr[5], matPtr[6], matPtr[7]);
	printf("    %f   %f   %f   %f\n", matPtr[8], matPtr[9], matPtr[10], matPtr[11]);
	printf("    %f   %f   %f   %f\n", matPtr[12], matPtr[13], matPtr[14], matPtr[15]);
}

void printMatrix(std::string matName, glm::mat3 mat)
{
	auto matPtr = glm::value_ptr(mat);
	printf("\n%s:\n", matName.c_str());
	printf("    %f   %f   %f\n", matPtr[0], matPtr[1], matPtr[2]);
	printf("    %f   %f   %f\n", matPtr[3], matPtr[4], matPtr[5]);
	printf("    %f   %f   %f\n", matPtr[6], matPtr[7], matPtr[8]);
}

void Shader::update(const MatrixStack * matrixStack)
{
	glm::mat4 modelMatrix = matrixStack->getMatrix();
	Camera* camera = CameraManager::getInstance().getCurrentCamera();

	if (m_uniforms[TRANSFORM_MVP] >= 0)
	{
		glm::mat4 mvpMatrix = camera->getInverseViewProjectionMatrix() * modelMatrix;
		glUniformMatrix4fv(m_uniforms[TRANSFORM_MVP], 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	}

	if (m_uniforms[TRANSFORM_MV] >= 0)
	{
		glm::mat4 mvMatrix = camera->getViewMatrix() * modelMatrix;
		glUniformMatrix4fv(m_uniforms[TRANSFORM_MV], 1, GL_FALSE, glm::value_ptr(mvMatrix));
	}

	if (m_uniforms[TRANSFORM_NORMAL] >= 0)
	{
		glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(camera->getViewMatrix() * modelMatrix)));
		glUniformMatrix3fv(m_uniforms[TRANSFORM_NORMAL], 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	if (m_uniforms[TRANSFORM_PROJECTION] >= 0) {
		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		glUniformMatrix4fv(m_uniforms[TRANSFORM_PROJECTION], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

	if (m_uniforms[TRANSFORM_MODEL] >= 0) {
		glUniformMatrix4fv(m_uniforms[TRANSFORM_MODEL], 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}
}

void Shader::update(const glm::mat4 matrix)
{
	glUniformMatrix4fv(m_uniforms[TRANSFORM_MVP], 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setFloat(std::string name, float val)
{
	glUniform1f(getUniformName(name), val);
}

void Shader::setColorRGB(std::string name, Color color)
{
	glUniform3f(getUniformName(name), color.r, color.g, color.b);
}

void Shader::setColorRGBA(std::string name, Color color)
{
	glUniform4f(getUniformName(name), color.r, color.g, color.b, color.a);
}

void Shader::setInt(std::string name, int val)
{
	glUniform1i(getUniformName(name), val);
}

void Shader::setVec3(std::string name, glm::vec3 val)
{
	glUniform3f(getUniformName(name), val.x, val.y, val.z);
}

void Shader::setVec4(std::string name, glm::vec4 val)
{
	int glName = getUniformName(name);
	glUniform4fv(glName, 1, glm::value_ptr(val));
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

		std::cerr << errorMessage << ": '" << std::endl << error << "'" << std::endl;
	}
}