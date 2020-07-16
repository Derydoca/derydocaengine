#include "EngineComponentsPch.h"
#include "Components\Light.h"

#include <GL\glew.h>
#include <iostream>
#include "Rendering\LightManager.h"
#include "Rendering\Material.h"
#include "Rendering\GraphicsAPI.h"

namespace DerydocaEngine::Components
{

	Light::Light() :
		m_Type(LightType::Point),
		m_ShadowMapFilterType(ShadowMapFilterType::Nearest),
		m_ShadowMapSize(512, 512),
		m_Color(1, 1, 1, 1),
		m_CastShadows(false),
		m_SpotlightExponent(0.0f),
		m_SpotlightCutoff(0.0f),
		m_ShadowSoftness(0.01f),
		m_DepthTexture(0),
		m_ShadowFBO(0),
		m_MatrixStack(std::make_shared<Rendering::MatrixStack>()),
		m_ShadowMapMaterial(nullptr),
		m_Projection(),
		m_ShadowBias()
	{
	}

	Light::~Light()
	{
	}

	glm::mat4 Light::getProjectionMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 Light::getViewMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 Light::getShadowMatrix()
	{
		glm::mat4 model = getGameObject()->getTransform()->getModel();
		return (m_ShadowBias * m_Projection.getProjectionMatrix() * m_Projection.getViewMatrix(model));
	}

	void Light::init()
	{
		auto sp = std::static_pointer_cast<Light>(shared_from_this());
		Rendering::LightManager::getInstance().addLight(sp);

		if (m_CastShadows) {
			generateShadowMap();
		}
	}

	void Light::preDestroy()
	{
		auto sp = std::static_pointer_cast<Light>(shared_from_this());
		Rendering::LightManager::getInstance().removeLight(sp);
	}

	void Light::renderShadowMap(const std::vector<std::shared_ptr<Scenes::Scene>> scenes)
	{
		auto prevFramebufferId = Rendering::GraphicsAPI::getCurrentFramebufferID();
		Rendering::GraphicsAPI::bindFramebuffer(m_ShadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_ShadowMapSize.x, m_ShadowMapSize.y);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.5f, 10.0f);

		// Draw all meshes with the shadow map shader to the framebuffer
		m_ShadowMapMaterial->bind();
		std::shared_ptr<Components::Transform> trans = getGameObject()->getTransform();
		for (auto scene : scenes)
		{
			scene->getRoot()->renderMesh(m_MatrixStack, m_ShadowMapMaterial, m_Projection, trans);
		}

		glCullFace(GL_BACK);
		glPolygonOffset(0.0f, 0.0f);
		glFlush();
		Rendering::GraphicsAPI::bindFramebuffer(prevFramebufferId);
	}

	void Light::generateShadowMap()
	{
		GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };

		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glFinish();
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, m_ShadowMapSize.x, m_ShadowMapSize.y);
		GLint filterType = getShadowMapFilterTypeEnum();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

		glGenFramebuffers(1, &m_ShadowFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

		GLenum drawBuffers[] = { GL_NONE };
		glDrawBuffers(1, drawBuffers);

		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			D_LOG_ERROR("Framebuffer is not complete.");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		boost::uuids::string_generator gen;
		boost::uuids::uuid shaderId = gen("05cdcea1-2312-4e30-828c-68717d484274");
		auto shadowMapShader = getResourcePointer<Rendering::Shader>(shaderId);
		m_ShadowMapMaterial = std::make_shared<Rendering::Material>();
		m_ShadowMapMaterial->setShader(shadowMapShader);

		m_ShadowBias = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
			glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		m_Projection.setAspectRatio(1.0);
		m_Projection.setFov(70.0f);
		m_Projection.setZFar(100.0f);
		m_Projection.setZNear(0.01f);
		m_Projection.recalculateProjectionMatrix();
	}

	int Light::getShadowMapFilterTypeEnum()
	{
		switch (m_ShadowMapFilterType)
		{
		case ShadowMapFilterType::Linear:
			return GL_LINEAR;
		case ShadowMapFilterType::Nearest:
			return GL_NEAREST;
		default:
			return GL_NEAREST;
		}
	}

	SERIALIZE_FUNC_LOAD(archive, Light)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Type),
			SERIALIZE(m_ShadowMapFilterType),
			SERIALIZE(m_ShadowMapSize),
			SERIALIZE(m_Color),
			SERIALIZE(m_CastShadows),
			SERIALIZE(m_SpotlightExponent),
			SERIALIZE(m_SpotlightCutoff),
			SERIALIZE(m_ShadowSoftness)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, Light)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Type),
			SERIALIZE(m_ShadowMapFilterType),
			SERIALIZE(m_ShadowMapSize),
			SERIALIZE(m_Color),
			SERIALIZE(m_CastShadows),
			SERIALIZE(m_SpotlightExponent),
			SERIALIZE(m_SpotlightCutoff),
			SERIALIZE(m_ShadowSoftness)
		);
	}

}
