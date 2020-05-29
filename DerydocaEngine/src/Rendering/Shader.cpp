#include "EnginePch.h"
#include "Rendering\Shader.h"

#include <fstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <sys/stat.h>
#include "Components\Camera.h"
#include "Rendering\CameraManager.h"
#include "Debug\DebugVisualizer.h"
#include "Rendering\GraphicsAPI.h"
#include "Rendering\MatrixStack.h"
#include "Rendering\Mesh.h"
#include "Rendering\RenderPass.h"
#include "Rendering\RenderTexture.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Texture.h"
#include "Components\Transform.h"
#include "..\Debug\GLError.h"

namespace DerydocaEngine::Rendering
{

#define LOAD_SHADER_IF_EXISTS(shaderIndex, shaderType, fileName) \
{ \
	if(CheckIfShaderExists(fileName)) \
	{ \
		m_shaders[(shaderIndex)] = CreateShader(LoadShader(fileName), shaderType); \
	} \
}
	Shader::Shader(const std::string& fileName) :
		m_rendererId(0),
		m_shaders(),
		m_uniforms(),
		m_loadPath(fileName),
		m_uniformLookup(),
		m_numPasses(0),
		m_renderPasses()
	{
		D_LOG_TRACE("Loading shader: {}", fileName);

		// Create the shaders
		LOAD_SHADER_IF_EXISTS(0, GL_VERTEX_SHADER, GetVertexShaderPath());
		LOAD_SHADER_IF_EXISTS(1, GL_TESS_CONTROL_SHADER, GetTesslleationControlShaderPath());
		LOAD_SHADER_IF_EXISTS(2, GL_TESS_EVALUATION_SHADER, GetTessellationEvaluationShaderPath());
		LOAD_SHADER_IF_EXISTS(3, GL_GEOMETRY_SHADER, GetGeometryShaderPath());
		LOAD_SHADER_IF_EXISTS(4, GL_FRAGMENT_SHADER, GetFragmentShaderPath());

		// Create the program
		m_rendererId = glCreateProgram();
		if (0 == m_rendererId)
		{
			D_LOG_ERROR("Error creating program object.");
			__debugbreak();
		}

		// Attach the shaders to the program
		for (unsigned int i = 0; i < NUM_SHADERS; i++) {
			// Skip shaders that were not loaded (i.e. shaders w/o geom shaders)
			if (m_shaders[i] == 0)
			{
				continue;
			}

			glAttachShader(m_rendererId, m_shaders[i]);
		}

		// Get the vertex attribute locations
		glBindAttribLocation(m_rendererId, 0, "VertexPosition");
		glBindAttribLocation(m_rendererId, 1, "VertexTexCoord");
		glBindAttribLocation(m_rendererId, 2, "VertexNormal");
		glBindAttribLocation(m_rendererId, 3, "VertexTangent");
		glBindAttribLocation(m_rendererId, 4, "VertexBitangent");
		glBindAttribLocation(m_rendererId, 5, "VertexColor");
		glBindAttribLocation(m_rendererId, 6, "VertexBoneIndices");
		glBindAttribLocation(m_rendererId, 7, "VertexBoneWeights");

		// Bind the output color to 0
		glBindFragDataLocation(m_rendererId, 0, "FragColor");

		// Link the program
		glLinkProgram(m_rendererId);
		if (!CheckShaderError(m_rendererId, GL_LINK_STATUS, true, "Error: Program linking failed"))
		{
			deleteShaderProgram();
			return;
		}

		glValidateProgram(m_rendererId);
		if (!CheckShaderError(m_rendererId, GL_VALIDATE_STATUS, true, "Error: Program is invalid"))
		{
			deleteShaderProgram();
			return;
		}

		m_uniforms[TRANSFORM_MVP] = GraphicsAPI::getUniformLocation(m_rendererId, "MVP");
		m_uniforms[TRANSFORM_MV] = GraphicsAPI::getUniformLocation(m_rendererId, "ModelViewMatrix");
		m_uniforms[TRANSFORM_NORMAL] = GraphicsAPI::getUniformLocation(m_rendererId, "NormalMatrix");
		m_uniforms[TRANSFORM_PROJECTION] = GraphicsAPI::getUniformLocation(m_rendererId, "ProjectionMatrix");
		m_uniforms[TRANSFORM_MODEL] = GraphicsAPI::getUniformLocation(m_rendererId, "ModelMatrix");
		m_uniforms[TRANSFORM_VIEW] = GraphicsAPI::getUniformLocation(m_rendererId, "ViewMatrix");
	}

	Shader::Shader(const std::string& fileName, const int varyingsCount, const char * const * varyings) :
		m_rendererId(0),
		m_shaders(),
		m_uniforms(),
		m_loadPath(fileName),
		m_uniformLookup(),
		m_numPasses(0),
		m_renderPasses()
	{
		D_LOG_TRACE("Loading shader: {}", fileName);

		// Create the shaders
		LOAD_SHADER_IF_EXISTS(0, GL_VERTEX_SHADER, GetVertexShaderPath());
		LOAD_SHADER_IF_EXISTS(1, GL_TESS_CONTROL_SHADER, GetTesslleationControlShaderPath());
		LOAD_SHADER_IF_EXISTS(2, GL_TESS_EVALUATION_SHADER, GetTessellationEvaluationShaderPath());
		LOAD_SHADER_IF_EXISTS(3, GL_GEOMETRY_SHADER, GetGeometryShaderPath());
		LOAD_SHADER_IF_EXISTS(4, GL_FRAGMENT_SHADER, GetFragmentShaderPath());

		// Create the program
		m_rendererId = glCreateProgram();
		if (0 == m_rendererId)
		{
			D_LOG_ERROR("Error creating program object.");
			__debugbreak();
		}

		// Attach the shaders to the program
		for (unsigned int i = 0; i < NUM_SHADERS; i++) {
			// Skip shaders that were not loaded (i.e. shaders w/o geom shaders)
			if (m_shaders[i] == 0)
			{
				continue;
			}

			glAttachShader(m_rendererId, m_shaders[i]);
		}

		// Get the vertex attribute locations
		glBindAttribLocation(m_rendererId, 0, "VertexPosition");
		glBindAttribLocation(m_rendererId, 1, "VertexTexCoord");
		glBindAttribLocation(m_rendererId, 2, "VertexNormal");
		glBindAttribLocation(m_rendererId, 3, "VertexTangent");
		glBindAttribLocation(m_rendererId, 4, "VertexBitangent");
		glBindAttribLocation(m_rendererId, 5, "VertexColor");
		glBindAttribLocation(m_rendererId, 6, "VertexBoneIndices");
		glBindAttribLocation(m_rendererId, 7, "VertexBoneWeights");

		// Bind the output color to 0
		glBindFragDataLocation(m_rendererId, 0, "FragColor");

		// Bind the varyings
		setTransformFeedbackVaryings(varyingsCount, varyings);

		// Link the program
		glLinkProgram(m_rendererId);
		if(!CheckShaderError(m_rendererId, GL_LINK_STATUS, true, "Error: Program linking failed: "))
		{
			deleteShaderProgram();
			return;
		}

		glValidateProgram(m_rendererId);
		if(!CheckShaderError(m_rendererId, GL_VALIDATE_STATUS, true, "Error: Program is invalid: "))
		{
			deleteShaderProgram();
			return;
		}

		m_uniforms[TRANSFORM_MVP] = GraphicsAPI::getUniformLocation(m_rendererId, "MVP");
		m_uniforms[TRANSFORM_MV] = GraphicsAPI::getUniformLocation(m_rendererId, "ModelViewMatrix");
		m_uniforms[TRANSFORM_NORMAL] = GraphicsAPI::getUniformLocation(m_rendererId, "NormalMatrix");
		m_uniforms[TRANSFORM_PROJECTION] = GraphicsAPI::getUniformLocation(m_rendererId, "ProjectionMatrix");
		m_uniforms[TRANSFORM_MODEL] = GraphicsAPI::getUniformLocation(m_rendererId, "ModelMatrix");
		m_uniforms[TRANSFORM_VIEW] = GraphicsAPI::getUniformLocation(m_rendererId, "ViewMatrix");
	}

	Shader::~Shader()
	{
		for (unsigned int i = 0; i < NUM_SHADERS; i++) {
			glDetachShader(m_rendererId, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}

		deleteShaderProgram();

		delete[] m_renderPasses;
	}

	void Shader::bind()
	{
		glUseProgram(m_rendererId);
	}

	void Shader::update(
		const std::shared_ptr<MatrixStack>& matrixStack,
		const Projection& projection,
		const std::shared_ptr<Components::Transform> trans)
	{
		glm::mat4 modelMatrix = matrixStack->getMatrix();
		glm::mat4 transformModelMatrix = trans->getModel();

		if (m_uniforms[TRANSFORM_MVP] >= 0)
		{
			glm::mat4 mvpMatrix = projection.getInverseViewProjectionMatrix(transformModelMatrix) * modelMatrix;
			GraphicsAPI::setUniformMat4(m_uniforms[TRANSFORM_MVP], glm::value_ptr(mvpMatrix), 1);
		}

		if (m_uniforms[TRANSFORM_MV] >= 0)
		{
			glm::mat4 mvMatrix = projection.getViewMatrix(transformModelMatrix) * modelMatrix;
			GraphicsAPI::setUniformMat4(m_uniforms[TRANSFORM_MV], glm::value_ptr(mvMatrix), 1);
		}

		if (m_uniforms[TRANSFORM_NORMAL] >= 0)
		{
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(projection.getViewMatrix(transformModelMatrix) * modelMatrix)));
			GraphicsAPI::setUniformMat3(m_uniforms[TRANSFORM_NORMAL], glm::value_ptr(normalMatrix), 1);
		}

		if (m_uniforms[TRANSFORM_PROJECTION] >= 0) {
			glm::mat4 projectionMatrix = projection.getProjectionMatrix();
			GraphicsAPI::setUniformMat4(m_uniforms[TRANSFORM_PROJECTION], glm::value_ptr(projectionMatrix), 1);
		}

		if (m_uniforms[TRANSFORM_MODEL] >= 0) {
			GraphicsAPI::setUniformMat4(m_uniforms[TRANSFORM_MODEL], glm::value_ptr(modelMatrix), 1);
		}

		if (m_uniforms[TRANSFORM_VIEW] > 0)
		{
			GraphicsAPI::setUniformMat4(m_uniforms[TRANSFORM_VIEW], glm::value_ptr(projection.getViewMatrix(transformModelMatrix)), 1);
		}

		glm::vec3 worldCamPos = trans->getWorldPosition();
		int worldCamPosUniformLocation = getUniformLocation("WorldCameraPosition");
		GraphicsAPI::setUniform(worldCamPosUniformLocation, worldCamPos.x, worldCamPos.y, worldCamPos.z);
	}

	void Shader::update(const glm::mat4& matrix)
	{
		GraphicsAPI::setUniformMat4(m_uniforms[TRANSFORM_MVP], &matrix[0][0], 1);
	}

	void Shader::updateViaActiveCamera(const std::shared_ptr<MatrixStack>& matrixStack)
	{
		auto camera = CameraManager::getInstance().getCurrentCamera();

		update(matrixStack, camera->getProjection(), camera->getGameObject()->getTransform());

		float w2 = camera->getDisplayWidth() / 2;
		float h2 = camera->getDisplayHeight() / 2;
		glm::mat4 viewportMatrix = glm::mat4(glm::vec4(w2, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, h2, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
			glm::vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
		int viewportMatrixUniformLocation = getUniformLocation("ViewportMatrix");
		GraphicsAPI::setUniformMat4(viewportMatrixUniformLocation, glm::value_ptr(viewportMatrix), 1);
	}

	void Shader::bindUniformBuffer(const std::string & name, int uniformBufferId)
	{
		unsigned int blockIndex = glGetUniformBlockIndex(m_rendererId, name.c_str());
		if (blockIndex != GL_INVALID_INDEX)
		{
			GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uniformBufferId));
		}
	}

	void Shader::setFloat(const std::string& name, const float val)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, val);
	}

	void Shader::setFloatArray(const std::string& name, const std::vector<float>& value)
	{
		for (size_t i = 0; i < value.size(); i++)
		{
			std::string uniformStringName = name + "[" + std::to_string(i) + "]";
			int uniformLocation = getUniformLocation(uniformStringName);
			GraphicsAPI::setUniform(uniformLocation, value[i]);
		}
	}

	void Shader::setFloatArray(const std::string& name, const float * arrayLocation, const unsigned int arrayLength)
	{
		for (unsigned int i = 0; i < arrayLength; i++)
		{
			std::string uniformStringName = name + "[" + std::to_string(i) + "]";
			int uniformLocation = getUniformLocation(uniformStringName);
			GraphicsAPI::setUniform(uniformLocation, arrayLocation[i]);
		}
	}

	void Shader::setColorRGB(const std::string& name, const Color& color)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, color.r, color.g, color.b);
	}

	void Shader::setColorRGBA(const std::string& name, const Color& color)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, color.r, color.g, color.b, color.a);
	}

	void Shader::setInt(const std::string& name, const int val)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, val);
	}

	void Shader::setIntArray(const std::string& name, const int * arrayLocation, const unsigned int arrayLength)
	{
		for (unsigned int i = 0; i < arrayLength; i++)
		{
			std::string uniformStringName = name + std::to_string(i);
			int uniformLocation = getUniformLocation(uniformStringName);
			GraphicsAPI::setUniform(uniformLocation, arrayLocation[i]);
		}
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& val)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, val.x, val.y, val.z);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4& val)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, val.x, val.y, val.z, val.w);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3& val)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniformMat3(uniformLocation, &val[0][0], 1);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& val)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniformMat4(uniformLocation, &val[0][0], 1);
	}

	void Shader::setMat4Array(const std::string& name, const std::vector<glm::mat4>& valArray)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniformMat4(uniformLocation, &valArray.at(0)[0][0], static_cast<int>(valArray.size()));
	}

	void Shader::setTexture(const std::string& name, const int textureUnit, const std::shared_ptr<Rendering::Texture> texture)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setTexture(uniformLocation, textureUnit, texture->getTextureType(), texture->getRendererId());
	}

	void Shader::setTexture(const std::string& name, const int textureUnit, const unsigned int textureType, const unsigned int textureId)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setTexture(uniformLocation, textureUnit, textureType, textureId);
	}

	void Shader::setTransformFeedbackVaryings(const int count, const char *const * varyings)
	{
		glTransformFeedbackVaryings(m_rendererId, count, varyings, GL_SEPARATE_ATTRIBS);
	}

	void Shader::deleteShaderProgram()
	{
		glDeleteProgram(m_rendererId);
		m_rendererId = 0;
	}

	unsigned int Shader::getSubroutineIndex(const unsigned int program, const std::string& subroutineName)
	{
		return glGetSubroutineIndex(m_rendererId, program, subroutineName.c_str());
	}

	void Shader::setSubroutine(const unsigned int program, const unsigned int subroutineIndex)
	{
		glUniformSubroutinesuiv(program, 1, &subroutineIndex);
	}

	void Shader::setSubPasses(const unsigned int program, RenderPass* renderPasses, const int numPasses)
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

	void Shader::renderMesh(const std::shared_ptr<Mesh> mesh, const std::shared_ptr<RenderTexture> m_renderTexture)
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
					GraphicsAPI::bindFramebuffer(0);
					//cout << "No proper render target was supplied!\n";
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

	int Shader::getUniformLocation(const std::string& stringName)
	{
		if (m_uniformLookup.count(stringName) != 0)
		{
			return m_uniformLookup[stringName];
		}
		else
		{
			int uniformName = GraphicsAPI::getUniformLocation(m_rendererId, stringName);
			m_uniformLookup[stringName] = uniformName;
			return uniformName;
		}
	}

	unsigned int Shader::CreateShader(const std::string& text, const unsigned int shaderType) {
		GLuint shader = glCreateShader(shaderType);

		if (shader == 0) {
			D_LOG_ERROR("Shader creation failed!");
		}

		const GLchar* shaderSourceStrings[1];
		GLint shaderSourceStringLengths[1];

		shaderSourceStrings[0] = text.c_str();
		shaderSourceStringLengths[0] = static_cast<GLint>(text.length());

		glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
		glCompileShader(shader);

		if (!CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: "))
		{
			return getErrorShaderID();
		}

		return shader;
	}

	int Shader::getErrorShaderID()
	{
		auto errorShader = Rendering::ShaderLibrary::getInstance().getErrorShader();
		return errorShader->m_rendererId;
	}

	std::string Shader::LoadShader(const std::string& fileName) {
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
			D_LOG_ERROR("Unable to load shader: {}", fileName);
		}

		return output;
	}

	bool Shader::CheckIfShaderExists(const std::string& fileName)
	{
		struct stat buffer;
		return (stat(fileName.c_str(), &buffer) == 0);
	}

	bool Shader::CheckShaderError(const unsigned int shader, const unsigned int flag, const bool isProgram, const std::string& errorMessage) {
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

			D_LOG_ERROR("{}:\n{}", errorMessage, error);
			
			return false;
		}

		return true;
	}

	void Shader::clearFloat(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, 0.0f);
	}

	void Shader::clearFloatArray(const std::string& name, const unsigned int arrayLength)
	{
		for (unsigned int i = 0; i < arrayLength; i++)
		{
			std::string uniformStringName = name + "[" + std::to_string(i) + "]";
			int uniformLocation = getUniformLocation(uniformStringName);
			GraphicsAPI::setUniform(uniformLocation, 0.0f);
		}
	}

	void Shader::clearColorRGB(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, 0.0f, 0.0f, 0.0f);
	}

	void Shader::clearColorRGBA(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	void Shader::clearInt(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, 0);
	}

	void Shader::clearIntArray(const std::string& name, unsigned int arrayLength)
	{
		for (unsigned int i = 0; i < arrayLength; i++)
		{
			std::string uniformStringName = name + std::to_string(i);
			int uniformLocation = getUniformLocation(uniformStringName);
			GraphicsAPI::setUniform(uniformLocation, 0);
		}
	}

	void Shader::clearVec3(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, 0.0f, 0.0f, 0.0f);
	}

	void Shader::clearVec4(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setUniform(uniformLocation, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	void Shader::clearMat3(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		float emptyMatrix[3 * 3] = { 0 };
		GraphicsAPI::setUniformMat3(uniformLocation, emptyMatrix, 1);
	}

	void Shader::clearMat4(const std::string& name)
	{
		int uniformLocation = getUniformLocation(name);
		float emptyMatrix[4 * 4] = { 0 };
		GraphicsAPI::setUniformMat4(uniformLocation, emptyMatrix, 1);
	}

	void Shader::clearTexture(const std::string& name, const int textureUnit, const unsigned int textureType)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setTexture(uniformLocation, textureUnit, textureType, 0);
	}

}
