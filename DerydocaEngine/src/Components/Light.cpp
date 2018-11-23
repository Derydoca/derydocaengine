#include "Components\Light.h"

#include <GL\glew.h>
#include <iostream>
#include "LightManager.h"
#include "Material.h"

namespace DerydocaEngine::Components
{

	Light::Light()
	{
		Rendering::LightManager::getInstance().addLight(this);
	}

	Light::~Light()
	{
		Rendering::LightManager::getInstance().removeLight(this);
		delete m_shadowMapMaterial;
	}

	glm::mat4 Light::getProjectionMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 Light::getViewMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 Light::getShadowMatrix(glm::mat4 const& objectModelMatrix)
	{
		glm::mat4 model = getGameObject()->getTransform()->getModel();
		return (m_shadowBias * m_projection.getProjectionMatrix() * m_projection.getViewMatrix(model)) * objectModelMatrix;
	}

	void Light::init()
	{
		if (m_castShadows) {
			generateShadowMap();
		}
	}

	void Light::deserialize(YAML::Node const& node)
	{
		Color lightColor = node["color"].as<Color>();
		setColor(&lightColor);

		LightType type = (LightType)node["type"].as<int>();
		setLightType(type);

		YAML::Node spotlightExponentNode = node["spotlightExponent"];
		if (spotlightExponentNode)
		{
			m_spotlightExponent = spotlightExponentNode.as<float>();
		}

		YAML::Node spotlightCutoffNode = node["spotlightCutoff"];
		if (spotlightCutoffNode)
		{
			m_spotlightCutoff = spotlightCutoffNode.as<float>();
		}

		YAML::Node castShadowsNode = node["castShadows"];
		if (castShadowsNode)
		{
			m_castShadows = castShadowsNode.as<bool>();
		}

		YAML::Node shadowMapHeightNode = node["shadowMapHeight"];
		if (shadowMapHeightNode)
		{
			m_shadowMapHeight = shadowMapHeightNode.as<int>();
		}

		YAML::Node shadowMapWidthNode = node["shadowMapWidth"];
		if (shadowMapWidthNode)
		{
			m_shadowMapWidth = shadowMapWidthNode.as<int>();
		}

		YAML::Node shadowMapFilterTypeNode = node["shadowMapFilterType"];
		if (shadowMapFilterTypeNode)
		{
			m_shadowMapFilterType = (ShadowMapFilterType)shadowMapFilterTypeNode.as<int>();
		}

		YAML::Node shadowSoftnessNode = node["shadowSoftness"];
		if (shadowSoftnessNode)
		{
			m_shadowSoftness = shadowSoftnessNode.as<float>();
		}
	}

	void Light::renderShadowMap(GameObject* const& gameObject)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_shadowMapWidth, m_shadowMapHeight);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.5f, 10.0f);

		// Draw all meshes with the shadow map shader to the framebuffer
		m_shadowMapMaterial->bind();
		Transform* trans = getGameObject()->getTransform();
		gameObject->renderMesh(&m_matrixStack, m_shadowMapMaterial, m_projection, trans);

		glCullFace(GL_BACK);
		glPolygonOffset(0.0f, 0.0f);
		glFlush();
	}

	void Light::generateShadowMap()
	{
		GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };

		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glFinish();
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, m_shadowMapWidth, m_shadowMapHeight);
		GLint filterType = getShadowMapFilterTypeEnum();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);

		glGenFramebuffers(1, &m_shadowFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		GLenum drawBuffers[] = { GL_NONE };
		glDrawBuffers(1, drawBuffers);

		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result == GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer is complete.\n";
		}
		else
		{
			std::cout << "Framebuffer is not complete.\n";
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		boost::uuids::string_generator gen;
		boost::uuids::uuid shaderId = gen("05cdcea1-2312-4e30-828c-68717d484274");
		Rendering::Shader* shadowMapShader = getResourceObject<Rendering::Shader>(shaderId);
		delete m_shadowMapMaterial;
		m_shadowMapMaterial = new Rendering::Material();
		m_shadowMapMaterial->setShader(shadowMapShader);

		m_shadowBias = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
			glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		m_projection.setAspectRatio(1.0);
		m_projection.setFov(70.0f);
		m_projection.setZFar(100.0f);
		m_projection.setZNear(0.01f);
		m_projection.recalculateProjectionMatrix();
	}

	int Light::getShadowMapFilterTypeEnum()
	{
		switch (m_shadowMapFilterType)
		{
		case ShadowMapFilterType::Linear:
			return GL_LINEAR;
		case ShadowMapFilterType::Nearest:
			return GL_NEAREST;
		default:
			return GL_NEAREST;
		}
	}

}
