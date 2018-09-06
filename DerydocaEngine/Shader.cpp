#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <glm/gtc/type_ptr.hpp>
#include "CameraManager.h"
#include "GLError.h"

#define LOAD_SHADER_IF_EXISTS(shaderIndex, shaderType, fileName) \
{ \
	if(CheckIfShaderExists(fileName)) \
	{ \
		m_shaders[(shaderIndex)] = CreateShader(LoadShader(fileName), shaderType); \
	} \
}

using namespace std;

static void CheckShaderError(GLuint const& shader, GLuint const& flag, bool const& isProgram, std::string const& errorMessage);
static std::string LoadShader(std::string const& fileName);
static bool CheckIfShaderExists(std::string const& fileName);
static GLuint CreateShader(std::string const& text, GLenum const& shaderType);

Shader::Shader(std::string const& fileName)
{
	m_loadPath = fileName;

	printf("Loading shader: %s\n", fileName.c_str());

	// Create the shaders
	LOAD_SHADER_IF_EXISTS(0, GL_VERTEX_SHADER, GetVertexShaderPath());
	LOAD_SHADER_IF_EXISTS(1, GL_TESS_CONTROL_SHADER, GetTesslleationControlShaderPath());
	LOAD_SHADER_IF_EXISTS(2, GL_TESS_EVALUATION_SHADER, GetTessellationEvaluationShaderPath());
	LOAD_SHADER_IF_EXISTS(3, GL_GEOMETRY_SHADER, GetGeometryShaderPath());
	LOAD_SHADER_IF_EXISTS(4, GL_FRAGMENT_SHADER, GetFragmentShaderPath());

	// Create the program
	m_program = glCreateProgram();
	if (0 == m_program)
	{
		fprintf(stderr, "Error creating program object..\n");
		__debugbreak();
	}

	// Attach the shaders to the program
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		// Skip shaders that were not loaded (i.e. shaders w/o geom shaders)
		if (m_shaders[i] == 0)
		{
			continue;
		}

		glAttachShader(m_program, m_shaders[i]);
	}

	// Get the vertex attribute locations
	glBindAttribLocation(m_program, 0, "VertexPosition");
	glBindAttribLocation(m_program, 1, "VertexTexCoord");
	glBindAttribLocation(m_program, 2, "VertexNormal");
	glBindAttribLocation(m_program, 3, "VertexTangent");
	glBindAttribLocation(m_program, 4, "VertexBitangent");
	glBindAttribLocation(m_program, 5, "VertexColor");

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

Shader::Shader(std::string const& fileName, int const& varyingsCount, const char * const * varyings)
{
	m_loadPath = fileName;

	printf("Loading shader: %s\n", fileName.c_str());

	// Create the shaders
	LOAD_SHADER_IF_EXISTS(0, GL_VERTEX_SHADER, GetVertexShaderPath());
	LOAD_SHADER_IF_EXISTS(1, GL_TESS_CONTROL_SHADER, GetTesslleationControlShaderPath());
	LOAD_SHADER_IF_EXISTS(2, GL_TESS_EVALUATION_SHADER, GetTessellationEvaluationShaderPath());
	LOAD_SHADER_IF_EXISTS(3, GL_GEOMETRY_SHADER, GetGeometryShaderPath());
	LOAD_SHADER_IF_EXISTS(4, GL_FRAGMENT_SHADER, GetFragmentShaderPath());

	// Create the program
	m_program = glCreateProgram();
	if (0 == m_program)
	{
		fprintf(stderr, "Error creating program object..\n");
		__debugbreak();
	}

	// Attach the shaders to the program
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		// Skip shaders that were not loaded (i.e. shaders w/o geom shaders)
		if (m_shaders[i] == 0)
		{
			continue;
		}

		glAttachShader(m_program, m_shaders[i]);
	}

	// Get the vertex attribute locations
	glBindAttribLocation(m_program, 0, "VertexPosition");
	glBindAttribLocation(m_program, 1, "VertexTexCoord");
	glBindAttribLocation(m_program, 2, "VertexNormal");
	glBindAttribLocation(m_program, 3, "VertexTangent");
	glBindAttribLocation(m_program, 4, "VertexBitangent");
	glBindAttribLocation(m_program, 5, "VertexColor");

	// Bind the output color to 0
	glBindFragDataLocation(m_program, 0, "FragColor");

	// Bind the varyings
	setTransformFeedbackVaryings(varyingsCount, varyings);

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

	delete[] m_renderPasses;
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void printMatrix(std::string const& matName, glm::mat4 const& mat)
{
	auto matPtr = glm::value_ptr(mat);
	printf("\n%s:\n", matName.c_str());
	printf("    %f   %f   %f   %f\n", matPtr[0], matPtr[1], matPtr[2], matPtr[3]);
	printf("    %f   %f   %f   %f\n", matPtr[4], matPtr[5], matPtr[6], matPtr[7]);
	printf("    %f   %f   %f   %f\n", matPtr[8], matPtr[9], matPtr[10], matPtr[11]);
	printf("    %f   %f   %f   %f\n", matPtr[12], matPtr[13], matPtr[14], matPtr[15]);
}

void printMatrix(std::string const& matName, glm::mat3 const& mat)
{
	auto matPtr = glm::value_ptr(mat);
	printf("\n%s:\n", matName.c_str());
	printf("    %f   %f   %f\n", matPtr[0], matPtr[1], matPtr[2]);
	printf("    %f   %f   %f\n", matPtr[3], matPtr[4], matPtr[5]);
	printf("    %f   %f   %f\n", matPtr[6], matPtr[7], matPtr[8]);
}

void Shader::update(MatrixStack * const& matrixStack, Projection const& projection, Transform* const& trans)
{
	glm::mat4 modelMatrix = matrixStack->getMatrix();
	glm::mat4 transformModelMatrix = trans->getModel();

	if (m_uniforms[TRANSFORM_MVP] >= 0)
	{
		glm::mat4 mvpMatrix = projection.getInverseViewProjectionMatrix(transformModelMatrix) * modelMatrix;
		glUniformMatrix4fv(m_uniforms[TRANSFORM_MVP], 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	}

	if (m_uniforms[TRANSFORM_MV] >= 0)
	{
		glm::mat4 mvMatrix = projection.getViewMatrix(transformModelMatrix) * modelMatrix;
		glUniformMatrix4fv(m_uniforms[TRANSFORM_MV], 1, GL_FALSE, glm::value_ptr(mvMatrix));
	}

	if (m_uniforms[TRANSFORM_NORMAL] >= 0)
	{
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(projection.getViewMatrix(transformModelMatrix) * modelMatrix)));
		glUniformMatrix3fv(m_uniforms[TRANSFORM_NORMAL], 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	if (m_uniforms[TRANSFORM_PROJECTION] >= 0) {
		glm::mat4 projectionMatrix = projection.getProjectionMatrix();
		glUniformMatrix4fv(m_uniforms[TRANSFORM_PROJECTION], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

	if (m_uniforms[TRANSFORM_MODEL] >= 0) {
		glUniformMatrix4fv(m_uniforms[TRANSFORM_MODEL], 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}

	glm::vec3 worldCamPos = trans->getWorldPos();
	glUniform3f(getUniformName("WorldCameraPosition"), worldCamPos.x, worldCamPos.y, worldCamPos.z);
}

void Shader::update(glm::mat4 const& matrix)
{
	glUniformMatrix4fv(m_uniforms[TRANSFORM_MVP], 1, GL_FALSE, &matrix[0][0]);
}

void Shader::updateViaActiveCamera(MatrixStack * const& matrixStack)
{
	Camera* camera = CameraManager::getInstance().getCurrentCamera();

	update(matrixStack, camera->getProjection(), camera->getGameObject()->getTransform());

	float w2 = camera->getDisplayWidth() / 2;
	float h2 = camera->getDisplayHeight() / 2;
	glm::mat4 viewportMatrix = glm::mat4(glm::vec4(w2, 0.0f, 0.0f, 0.0f),
	glm::vec4(0.0f, h2, 0.0f, 0.0f),
	glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
	glm::vec4(w2+0, h2+0, 0.0f, 1.0f));
	glUniformMatrix4fv(getUniformName("ViewportMatrix"), 1, GL_FALSE, glm::value_ptr(viewportMatrix));
}

void Shader::setFloat(std::string const& name, float const& val)
{
	glUniform1f(getUniformName(name), val);
}

void Shader::setFloatArray(std::string const& name, float * const& arrayLocation, unsigned int const& arrayLength)
{
	for (unsigned int i = 0; i < arrayLength; i++)
	{
		std::string uniformStringName = name + "[" + std::to_string(i) + "]";
		int uniformName = getUniformName(uniformStringName);
		glUniform1f(uniformName, arrayLocation[i]);
	}
}

void Shader::setColorRGB(std::string const& name, Color const& color)
{
	glUniform3f(getUniformName(name), color.r, color.g, color.b);
}

void Shader::setColorRGBA(std::string const& name, Color const& color)
{
	glUniform4f(getUniformName(name), color.r, color.g, color.b, color.a);
}

void Shader::setInt(std::string const& name, int const& val)
{
	glUniform1i(getUniformName(name), val);
}

void Shader::setIntArray(std::string const& name, int * const& arrayLocation, unsigned int const& arrayLength)
{
	for (unsigned int i = 0; i < arrayLength; i++)
	{
		std::string uniformStringName = name + std::to_string(i);
		int uniformName = getUniformName(uniformStringName);
		glUniform1d(uniformName, arrayLocation[i]);
	}
}

void Shader::setVec3(std::string const& name, glm::vec3 const& val)
{
	glUniform3f(getUniformName(name), val.x, val.y, val.z);
}

void Shader::setVec4(std::string const& name, glm::vec4 const& val)
{
	int glName = getUniformName(name);
	glUniform4fv(glName, 1, glm::value_ptr(val));
}

void Shader::setMat3(std::string const& name, glm::mat3 const& val)
{
	glUniformMatrix3fv(getUniformName(name), 1, GL_FALSE, &val[0][0]);
}

void Shader::setMat4(std::string const& name, glm::mat4 const& val)
{
	glUniformMatrix4fv(getUniformName(name), 1, GL_FALSE, &val[0][0]);
}

void Shader::setTexture(std::string const& name, int const& textureUnit, Texture* const& texture)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(texture->getTextureType(), texture->getTextureId());
	glUniform1i(getUniformName(name), textureUnit);
}

void Shader::setTexture(std::string const& name, int const& textureUnit, GLenum const& textureType, GLuint const& textureId)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(textureType, textureId);
	int uniformName = getUniformName(name);
	glUniform1i(uniformName, textureUnit);
}

void Shader::setTransformFeedbackVaryings(int const& count, const char *const * varyings)
{
	glTransformFeedbackVaryings(m_program, count, varyings, GL_SEPARATE_ATTRIBS);
}

GLuint Shader::getSubroutineIndex(GLuint const& program, std::string const& subroutineName)
{
	return glGetSubroutineIndex(m_program, program, subroutineName.c_str());
}

void Shader::setSubroutine(GLuint const& program, GLuint const& subroutineIndex)
{
	glUniformSubroutinesuiv(program, 1, &subroutineIndex);
}

void Shader::setSubPasses(GLuint const& program, RenderPass* const& renderPasses, int const& numPasses)
{
	m_numPasses = numPasses;

	delete[] m_renderPasses;
	m_renderPasses = new RenderPass[numPasses];
	for (int i = 0; i < numPasses; i++)
	{
		int subroutineIndex = getSubroutineIndex(program, renderPasses[i].getName());
		m_renderPasses[i] = RenderPass(&renderPasses[i], subroutineIndex);
	}
}

void Shader::renderMesh(Mesh * const& mesh, RenderTexture* const& m_renderTexture)
{
	if (m_numPasses <= 0)
	{
		mesh->draw();
	}
	else
	{
		for (int i = 0; i < m_numPasses; i++)
		{
			//int textureW = 1;
			//int textureH = 1;

			RenderPass rp = m_renderPasses[i];
			if (rp.hasRenderTextureAssigned())
			{
				rp.getRenderTexture()->bindAsRenderTexture();
				//textureW = renderPassRenderTexture->getWidth();
				//textureH = renderPassRenderTexture->getHeight();
			}
			else if (m_renderTexture != nullptr)
			{
				m_renderTexture->bindAsRenderTexture();
				//textureW = m_renderTexture->getWidth();
				//textureH = m_renderTexture->getHeight();
			}
			else
			{
				// Render to the screen
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				//cout << "No proper render target was supplied!" << endl;
			}

			// TODO: Add viewport suport
			//glViewport(
			//	(GLint)(textureW * m_displayRect->getX()),
			//	(GLint)(textureH * m_displayRect->getY()),
			//	(GLint)(textureW * m_displayRect->getWidth()),
			//	(GLint)(textureH * m_displayRect->getHeight()));

			setTexture("RenderTex", 0, m_renderTexture);
			setSubroutine(GL_FRAGMENT_SHADER, rp.getShaderSubroutineIndex());
			if (i > 0 && m_renderPasses[i - 1].hasRenderTextureAssigned())
			{
				setTexture(m_renderPasses[i - 1].getRenderTextureName(), 1, m_renderPasses[i - 1].getRenderTexture());
			}
			mesh->draw();
		}
	}
}

int Shader::getUniformName(std::string const& stringName)
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

static GLuint CreateShader(std::string const& text, GLenum const& shaderType) {
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

static std::string LoadShader(std::string const& fileName) {
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

static bool CheckIfShaderExists(std::string const& fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

static void CheckShaderError(GLuint const& shader, GLuint const& flag, bool const& isProgram, std::string const& errorMessage) {
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

void Shader::clearFloat(std::string const& name)
{
	glUniform1f(getUniformName(name), 0);
}

void Shader::clearFloatArray(std::string const& name, unsigned int const& arrayLength)
{
	for (unsigned int i = 0; i < arrayLength; i++)
	{
		std::string uniformStringName = name + "[" + std::to_string(i) + "]";
		int uniformName = getUniformName(uniformStringName);
		glUniform1f(uniformName, 0);
	}
}

void Shader::clearColorRGB(std::string const& name)
{
	glUniform3f(getUniformName(name), 0, 0, 0);
}

void Shader::clearColorRGBA(std::string const& name)
{
	glUniform4f(getUniformName(name), 0, 0, 0, 0);
}

void Shader::clearInt(std::string const& name)
{
	glUniform1i(getUniformName(name), 0);
}

void Shader::clearIntArray(std::string const& name, unsigned int const& arrayLength)
{
	for (unsigned int i = 0; i < arrayLength; i++)
	{
		std::string uniformStringName = name + std::to_string(i);
		int uniformName = getUniformName(uniformStringName);
		glUniform1d(uniformName, 0);
	}
}

void Shader::clearVec3(std::string const& name)
{
	glUniform3f(getUniformName(name), 0, 0, 0);
}

void Shader::clearVec4(std::string const& name)
{
	glUniform4f(getUniformName(name), 0, 0, 0, 0);
}

void Shader::clearMat3(std::string const& name)
{
	glUniformMatrix3fv(getUniformName(name), 1, GL_FALSE, &mat3()[0][0]);
}

void Shader::clearMat4(std::string const& name)
{
	glUniformMatrix4fv(getUniformName(name), 1, GL_FALSE, &mat4()[0][0]);
}

void Shader::clearTexture(std::string const& name, int const& textureUnit, GLenum const& textureType)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(textureType, 0);
	glUniform1i(getUniformName(name), textureUnit);
}
