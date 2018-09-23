#include "Camera.h"

#include <GL/glew.h>
#include "CameraManager.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "DisplayManager.h"
#include "MeshResource.h"
#include "glm\glm.hpp"
#include "Shader.h"
#include "ShaderLibrary.h"
#include "LightManager.h"
#include "Material.h"
#include "RenderTexture.h"
#include "Rectangle.h"
#include "Skybox.h"
#include "Mesh.h"

namespace DerydocaEngine::Components
{

	Camera::Camera()
	{
		Rendering::CameraManager::getInstance().addCamera(this);

		m_skybox = new Rendering::Skybox();
		m_displayRect = new Rectangle(0, 0, 1, 1);

		setDisplay(Rendering::DisplayManager::getInstance().getDisplay(0));
		m_projection.setAspectRatio(m_display->getAspectRatio());
		m_projection.recalculateProjectionMatrix();
		setClearMode(ClearMode::ColorClear);
		setClearColor(Color(0.5, 0.0, 0.0));
	}

	Camera::Camera(float const& fov, float const& aspect, float const& zNear, float const& zFar)
	{
		m_projection.setFov(fov);
		m_projection.setAspectRatio(aspect);
		m_projection.setZNear(zNear);
		m_projection.setZFar(zFar);
		m_projection.recalculateProjectionMatrix();

		Rendering::CameraManager::getInstance().addCamera(this);

		m_skybox = new Rendering::Skybox();
		m_displayRect = new Rectangle();
	}

	Camera::~Camera()
	{
		delete m_displayRect;
		delete m_skybox;
		delete m_renderTexture;
		delete m_postProcessMaterial;
		Rendering::CameraManager::getInstance().removeCamera(this);
	}

	void Camera::init()
	{
		Resources::MeshResource* quadResource = (Resources::MeshResource*)ObjectLibrary::getInstance().getResource("136a5d0f-51d7-4f3c-857c-0497de142a71");
		if (quadResource != nullptr)
		{
			m_quad = (Rendering::Mesh*)quadResource->getResourceObject();
		}

		m_transform = getGameObject()->getTransform();

		if (m_postProcessMaterial)
		{
			m_postProcessMaterial->setTexture("RenderTex", m_renderTexture);
		}
	}

	void Camera::setDisplayRect(float const& x, float const& y, float const& w, float const& h)
	{
		m_displayRect->setX(x);
		m_displayRect->setY(x);
		m_displayRect->setWidth(x);
		m_displayRect->setHeight(x);
	}

	void Camera::setRenderingMode(RenderingMode const& mode)
	{
		m_renderingMode = mode;

		if (m_renderingMode == RenderingMode::Deferred)
		{
			// initialize g-buffer

			int width = m_display->getWidth();
			int height = m_display->getHeight();

			glGenFramebuffers(1, &m_deferredFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);

			// Depth buffer
			glGenRenderbuffers(1, &m_gbuffDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, m_gbuffDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

			// Create the textures
			createGBufTex(GL_TEXTURE0, GL_RGB32F, m_gbuffPos, width, height);
			createGBufTex(GL_TEXTURE1, GL_RGB32F, m_gbuffNorm, width, height);
			createGBufTex(GL_TEXTURE2, GL_RGB8, m_gbuffColor, width, height);

			// Attach images to the framebuffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gbuffDepth);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gbuffPos, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gbuffNorm, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gbuffColor, 0);

			GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(4, drawBuffers);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// TODO: Find a better way to get the shader so the ID is not hard-coded
			m_deferredRendererCompositor = Rendering::ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\deferredRenderCompositorShader");
		}
		else
		{
			// destroy g-buffer
		}
	}

	void Camera::resize(int const& width, int const& height)
	{
		m_projection.setAspectRatio(width, height);
		m_projection.recalculateProjectionMatrix();
	}

	void Camera::createGBufTex(unsigned int const& textureUnit, unsigned int const& format, unsigned int &texid, int const& width, int const& height)
	{
		glActiveTexture(textureUnit);
		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}

	void Camera::setProjectionMode(Rendering::ProjectionMode const& mode)
	{
		m_projection.setProjectionMode(mode);
		m_projection.recalculateProjectionMatrix();
	}

	void Camera::setOrthoSize(float const& size)
	{
		m_orthoSize = size;
		m_projection.recalculateProjectionMatrix();
	}

	void Camera::deserialize(YAML::Node const& node)
	{
		float fov = node["fov"].as<float>();
		float zNear = node["zNear"].as<float>();
		float zFar = node["zFar"].as<float>();

		m_projection.setFov(fov);
		m_projection.setZNear(zNear);
		m_projection.setZFar(zFar);
		m_projection.recalculateProjectionMatrix();

		YAML::Node renderTextureNode = node["RenderTexture"];
		if (renderTextureNode)
		{
			int width = renderTextureNode["Width"].as<int>();
			int height = renderTextureNode["Height"].as<int>();
			m_renderTexture = new Rendering::RenderTexture(width, height);

			Rendering::Shader* postProcessingShader = getResourceObject<Rendering::Shader>(renderTextureNode, "PostProcessShader");
			m_postProcessMaterial = new Rendering::Material();
			m_postProcessMaterial->setShader(postProcessingShader);
		}
	}

	Rendering::Shader * Camera::getPostProcessShader() const
	{
		return m_postProcessMaterial->getShader();
	}

	void Camera::clear()
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		switch (m_clearMode)
		{
		case ColorClear:
			glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
			break;
		case SkyboxClear:
			assert(m_skyboxMaterial);

			m_skyboxMaterial->bind();
			m_skyboxMaterial->getShader()->update(m_projection.getRotationProjection(getGameObject()->getTransform()->getQuat()));
			m_skybox->getMesh()->draw();
			break;
		default:
			break;
		}
	}

	void Camera::renderRoot(GameObject* const& gameObject)
	{
		Rendering::LightManager::getInstance().renderShadowMaps(gameObject->getTransform());

		int textureW, textureH = 1;

		if (m_renderTexture)
		{
			m_renderTexture->bindAsRenderTexture();
			textureW = m_renderTexture->getWidth();
			textureH = m_renderTexture->getHeight();
		}
		else
		{
			m_display->bindAsRenderTarget();
			textureW = m_display->getWidth();
			textureH = m_display->getHeight();
		}

		if (m_renderingMode == RenderingMode::Deferred)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);
		}

		glViewport(
			(GLint)(textureW * m_displayRect->getX()),
			(GLint)(textureH * m_displayRect->getY()),
			(GLint)(textureW * m_displayRect->getWidth()),
			(GLint)(textureH * m_displayRect->getHeight()));
		glEnable(GL_DEPTH_TEST);
		clear();
		gameObject->preRender();
		gameObject->render(&m_matrixStack);

		// Postprocessing happens here
		if (m_renderTexture != nullptr && m_postProcessMaterial != nullptr)
		{
			glDisable(GL_DEPTH_TEST);

			m_postProcessMaterial->bind();

			// Load the shader with matricies that will transform the quad to take up the entire buffer
			Rendering::Shader* postProcessShader = m_postProcessMaterial->getShader();
			setIdentityMatricies(postProcessShader);

			postProcessShader->setInt("Width", m_renderTexture->getWidth());
			postProcessShader->setInt("Height", m_renderTexture->getHeight());
			//postProcessShader->setTexture("RenderTex", 0, m_renderTexture);

			// Render the full-buffer quad
			postProcessShader->renderMesh(m_quad, m_renderTexture);
		}

		// Deferred rendering happens here
		if (m_renderingMode == RenderingMode::Deferred)
		{
			// Render it to the screen
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);

			// Set the identity matrices so that the quad renders to the entire render area
			setIdentityMatricies(m_deferredRendererCompositor);

			// Render the full-buffer quad
			m_deferredRendererCompositor->setTexture("PositionTex", 0, GL_TEXTURE_2D, m_gbuffPos);
			m_deferredRendererCompositor->setTexture("NormalTex", 1, GL_TEXTURE_2D, m_gbuffNorm);
			m_deferredRendererCompositor->setTexture("ColorTex", 2, GL_TEXTURE_2D, m_gbuffColor);
			Rendering::LightManager::getInstance().bindLightsToShader(nullptr, getGameObject()->getTransform(), m_deferredRendererCompositor);
			m_deferredRendererCompositor->renderMesh(m_quad, nullptr);
		}
	}

	void Camera::setDisplay(Rendering::Display * const & display)
	{
		if (m_display != nullptr)
		{
			m_display->unregisterCamera();
		}

		m_display = display;

		if (m_display != nullptr)
		{
			m_display->registerCamera(this);
		}
	}

	float Camera::getDisplayWidth()
	{
		return m_renderTexture != nullptr ? (float)m_renderTexture->getWidth() : (float)m_display->getWidth();
	}

	float Camera::getDisplayHeight()
	{
		return m_renderTexture != nullptr ? (float)m_renderTexture->getHeight() : (float)m_display->getHeight();
	}

	void Camera::setIdentityMatricies(Rendering::Shader* const& shader)
	{
		glm::mat4 m = glm::mat4(1.0);
		glm::mat4 v = glm::mat4(1.0);
		glm::mat4 p = glm::mat4(1.0);
		glm::mat4 mv = v * m;
		glm::mat4 mvp = p * v * m;

		shader->bind();

		shader->setMat4("ModelViewMatrix", mv);
		shader->setMat3("NormalMatrix", glm::mat3(mv[0], glm::vec3(mv[1]), glm::vec3(mv[2])));
		shader->setMat4("MVP", mvp);
	}

}