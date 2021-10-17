#include "EnginePch.h"
#include "Rendering\Shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <sys/stat.h>
#include "Components\Camera.h"
#include "Rendering\CameraManager.h"
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
	void Shader::loadShaderIfExists(const size_t shaderIndex, const ShaderProgramType shaderType, const std::string& shaderPath)
	{
		if (CheckIfShaderExists(shaderPath))
		{
			m_shaders[shaderIndex] = GraphicsAPI::createShader(LoadShader(shaderPath), shaderType);
		}
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
		std::vector<ShaderAttribute> shaderAttributes = {
			{ 0, "VertexPosition" },
			{ 1, "VertexTexCoord" },
			{ 2, "VertexNormal" },
			{ 3, "VertexTangent" },
			{ 4, "VertexBitangent" },
			{ 5, "VertexColor" },
			{ 6, "VertexBoneIndices" },
			{ 7, "VertexBoneWeights" }
		};

		std::vector<ShaderAttribute> outputAttributes = {
			{ 0, "FragColor" }
		};

		D_LOG_TRACE("Loading shader: {}", fileName);

		// Create the shaders
		loadShaderIfExists(0, ShaderProgramType::VertexShader, GetVertexShaderPath());
		loadShaderIfExists(1, ShaderProgramType::TesselationControlShader, GetTesslleationControlShaderPath());
		loadShaderIfExists(2, ShaderProgramType::TesselationEvaluationShader, GetTessellationEvaluationShaderPath());
		loadShaderIfExists(3, ShaderProgramType::GeometryShader, GetGeometryShaderPath());
		loadShaderIfExists(4, ShaderProgramType::FragmentShader, GetFragmentShaderPath());

		m_rendererId = GraphicsAPI::createShaderProgram(m_shaders[0], m_shaders[1], m_shaders[2], m_shaders[3], m_shaders[4], shaderAttributes, outputAttributes);

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
			GraphicsAPI::deleteShader(m_rendererId, m_shaders[i]);
		}

		deleteShaderProgram();

		delete[] m_renderPasses;
	}

	void Shader::bind()
	{
		GraphicsAPI::bindShaderProgram(m_rendererId);
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
		GraphicsAPI::bindUniformBuffer(m_rendererId, name, uniformBufferId);
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

	void Shader::setTexture(const std::string& name, const int textureUnit, const TextureType textureType, const unsigned int textureId)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setTexture(uniformLocation, textureUnit, textureType, textureId);
	}

	void Shader::deleteShaderProgram()
	{
		GraphicsAPI::deleteShaderProgram(m_rendererId);
		m_rendererId = 0;
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

	uint32_t Shader::getErrorShaderID()
	{
		auto errorShader = Rendering::ShaderLibrary::getInstance().getErrorShader();
		return errorShader.As<Rendering::Shader>()->m_rendererId;
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

	void Shader::clearTexture(const std::string& name, const int textureUnit, const TextureType textureType)
	{
		int uniformLocation = getUniformLocation(name);
		GraphicsAPI::setTexture(uniformLocation, textureUnit, textureType, 0);
	}

}
