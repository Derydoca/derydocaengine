#include "EngineComponentsPch.h"
#include "Components\Camera.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Helpers\YamlTools.h"
#include "Rendering\CameraManager.h"
#include "Rendering\Display.h"
#include "Rendering\DisplayManager.h"
#include "Rendering\LightManager.h"
#include "Rendering\Material.h"
#include "Rendering\MatrixStack.h"
#include "Rendering\Mesh.h"
#include "Resources\MeshResource.h"
#include "Rectangle.h"
#include "Rendering\RenderTexture.h"
#include "Rendering\Shader.h"
#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Skybox.h"
#include "Rendering\GraphicsAPI.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Components
{

	Camera::Camera() : Camera(true)
	{
	}

	Camera::Camera(NoRegister) : Camera(false)
	{
	}

	Camera::Camera(bool registerWithManager) :
		m_transform(),
		m_clearColor(Color(0.0f, 0.0f, 0.2f, 1.0f)),
		m_skybox(std::make_shared<Rendering::Skybox>()),
		m_clearMode(Camera::NoClear),
		m_skyboxMaterial(nullptr),
		m_matrixStack(std::make_shared<Rendering::MatrixStack>()),
		m_renderTexture(nullptr),
		m_renderTextureDeferred(nullptr),
		m_display(nullptr),
		m_displayRect(Rectangle(0, 0, 1, 1)),
		m_quad(nullptr),
		m_postProcessMaterial(nullptr),
		m_deferredRendererCompositor(0),
		m_projection(),
		m_registerWithManager(registerWithManager)
	{
		setDisplay(Rendering::DisplayManager::getInstance().getDisplay(0));
		m_projection.setAspectRatio(m_display->getAspectRatio());
		m_projection.recalculateProjectionMatrix();
		setClearMode(ClearMode::ColorClear);
	}

	Camera::~Camera()
	{
	}

	void Camera::init()
	{
		if (m_registerWithManager)
		{
			Rendering::CameraManager::getInstance().addCamera(std::static_pointer_cast<Camera>(shared_from_this()));
		}

		auto quadResource = std::static_pointer_cast<Resources::MeshResource>(ObjectLibrary::getInstance().getResource("136a5d0f-51d7-4f3c-857c-0497de142a71"));
		if (quadResource != nullptr)
		{
			m_quad = std::static_pointer_cast<Rendering::Mesh>(quadResource->getResourceObjectPointer());
		}

		m_transform = getGameObject()->getTransform();

		if (m_postProcessMaterial)
		{
			m_postProcessMaterial->setTexture("RenderTex", m_renderTexture);
		}


	}

	void Camera::preDestroy()
	{
		if (m_registerWithManager)
		{
			Rendering::CameraManager::getInstance().removeCamera(std::static_pointer_cast<Camera>(shared_from_this()));
		}
	}

	void Camera::setDisplayRect(float const& x, float const& y, float const& w, float const& h)
	{
		m_displayRect.setX(x);
		m_displayRect.setY(x);
		m_displayRect.setWidth(x);
		m_displayRect.setHeight(x);
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

	void Camera::deserialize(const YAML::Node& node)
	{
		float fov = node["fov"].as<float>();
		float zNear = node["zNear"].as<float>();
		float zFar = node["zFar"].as<float>();

		m_projection.setFov(fov);
		m_projection.setZNear(zNear);
		m_projection.setZFar(zFar);
		m_projection.recalculateProjectionMatrix();

		auto renderingModeNode = node["renderingMode"];
		Rendering::RenderingMode renderingMode = Rendering::RenderingMode::Forward;
		if (renderingModeNode)
		{
			renderingMode = static_cast<Rendering::RenderingMode>(renderingModeNode.as<int>());
		}

		auto renderTextureNode = node["RenderTexture"];
		if (renderTextureNode)
		{
			int width = renderTextureNode["Width"].as<int>();
			int height = renderTextureNode["Height"].as<int>();
			m_renderTexture = std::make_shared<Rendering::RenderTexture>(Rendering::RenderingMode::Forward);
			m_renderTexture->initializeTexture(width, height);

			if (renderingMode == Rendering::RenderingMode::Deferred)
			{
				m_renderTextureDeferred = std::make_shared<Rendering::RenderTexture>(Rendering::RenderingMode::Deferred);
				m_renderTextureDeferred->initializeTexture(width, height);
			}

			auto postProcessingShader = getResourcePointer<Rendering::Shader>(renderTextureNode, "PostProcessShader");
			if (postProcessingShader)
			{
				m_postProcessMaterial = std::make_shared<Rendering::Material>();
				m_postProcessMaterial->setShader(postProcessingShader);
			}
		}

		auto clearModeNode = node["clearMode"];
		if (clearModeNode)
		{
			setClearMode(static_cast<ClearMode>(clearModeNode.as<int>()));
		}

		auto clearColorNode = node["clearColor"];
		if (clearColorNode)
		{
			setClearColor(clearColorNode.as<Color>());
		}

		auto skyboxNode = node["skybox"];
		if (skyboxNode)
		{
			auto skybox = getResourcePointer<Rendering::Material>(skyboxNode.as<boost::uuids::uuid>());
			setSkybox(skybox);
		}

		auto deferredCompositorShaderNode = node["deferredCompositorShader"];
		if (deferredCompositorShaderNode)
		{
			m_deferredRendererCompositor = getResourcePointer<Rendering::Shader>(deferredCompositorShaderNode.as<boost::uuids::uuid>());
		}
	}

	std::shared_ptr<Rendering::Shader> Camera::getPostProcessShader() const
	{
		return m_postProcessMaterial->getShader();
	}

	void Camera::clear()
	{
		// Clear the depth buffer
		Rendering::GraphicsAPI::clearDepthBuffer();

		// If the clear mode is set to ClearColor or SkyboxClear but no skybox is defined, clear it with the clear color
		if ((m_clearMode == ColorClear) || (m_clearMode == SkyboxClear && m_skyboxMaterial == nullptr))
		{
			Rendering::GraphicsAPI::clearColorBuffer(m_clearColor);
		}
		// Otherwise, if it passed the check before and is set to SkyboxClear then render the skybox
		else if (m_clearMode == SkyboxClear)
		{
			m_skyboxMaterial->bind();
			m_skyboxMaterial->getShader()->update(m_projection.getRotationProjection(getGameObject()->getTransform()->getQuat()));
			m_skybox->getMesh()->draw();
		}
		// NoClear will fall through and do nothing as we would expect it to do

	}

	void Camera::renderScenesToActiveBuffer(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, int textureW, int textureH)
	{
		// Set the viewport to what is defined on this camera
		Rendering::GraphicsAPI::setViewport(std::static_pointer_cast<Camera>(shared_from_this()), textureW, textureH);

		// Set the aspect ratio to the texture size
		m_projection.setAspectRatio(textureW, textureH);
		m_projection.recalculateProjectionMatrix();

		// Ensure depth testing is on
		glEnable(GL_DEPTH_TEST);

		// Clear the buffer
		clear();

		// Render each scene to the active buffer
		for (auto scene : scenes)
		{
			auto root = scene->getRoot();
			if (root == nullptr)
			{
				continue;
			}
			root->preRender();
			root->render(m_matrixStack);
		}
	}

	void Camera::renderToAttachedRenderTexture(const std::vector<std::shared_ptr<Scenes::Scene>> scenes)
	{
		if (!m_renderTexture)
		{
			return;
		}

		// Store the current framebuffer
		auto prevFramebufferId = Rendering::GraphicsAPI::getCurrentFramebufferID();

		auto rtex = m_renderTextureDeferred == nullptr ? m_renderTexture : m_renderTextureDeferred;

		rtex->bindAsRenderTexture();

		// Render the scene
		auto scene = Scenes::SceneManager::getInstance().getActiveScene();
		Rendering::CameraManager::getInstance().setCurrentCamera(std::static_pointer_cast<Camera>(shared_from_this()));
		renderScenesToActiveBuffer({ scene }, rtex->getWidth(), rtex->getHeight());

		// Deferred rendering happens here
		if (m_renderTextureDeferred)
		{
			assert(m_deferredRendererCompositor);

			// Render it to the render texture
			m_renderTexture->bindAsRenderTexture();

			Rendering::GraphicsAPI::clearDepthBuffer();
			Rendering::GraphicsAPI::clearColorBuffer({ 0.0f, 0.0f, 0.0f, 1.0f });
			glDisable(GL_DEPTH_TEST);

			// Set the identity matrices so that the quad renders to the entire render area
			setIdentityMatricies(m_deferredRendererCompositor);

			// Render the full-buffer quad
			//m_deferredRenderBuffer->bindBuffersToShader(m_deferredRendererCompositor);
			//m_deferredRendererCompositor->setTexture("PositionTex", 0, GL_TEXTURE_2D, m_gbuffPos);
			//m_deferredRendererCompositor->setTexture("NormalTex", 1, GL_TEXTURE_2D, m_gbuffNorm);
			//m_deferredRendererCompositor->setTexture("ColorTex", 2, GL_TEXTURE_2D, m_gbuffColor);
			m_renderTextureDeferred->bindDeferredTextures(m_deferredRendererCompositor);
			Rendering::LightManager::getInstance().bindLightsToShader(nullptr, getGameObject()->getTransform(), m_deferredRendererCompositor);
			m_deferredRendererCompositor->renderMesh(m_quad, nullptr);
		}

		// Postprocessing happens here
		if (m_postProcessMaterial != nullptr)
		{
			glDisable(GL_DEPTH_TEST);

			m_postProcessMaterial->bind();

			// Load the shader with matricies that will transform the quad to take up the entire buffer
			auto postProcessShader = m_postProcessMaterial->getShader();
			setIdentityMatricies(postProcessShader);

			postProcessShader->setInt("Width", m_renderTexture->getWidth());
			postProcessShader->setInt("Height", m_renderTexture->getHeight());

			// Render the full-buffer quad
			postProcessShader->renderMesh(m_quad, m_renderTexture);
		}

		// Rebind the old framebuffer
		Rendering::GraphicsAPI::bindFramebuffer(prevFramebufferId);
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

	void Camera::setIdentityMatricies(std::shared_ptr<Rendering::Shader> shader)
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