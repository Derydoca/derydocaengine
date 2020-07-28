#include "EngineComponentsPch.h"
#include "Components\Camera.h"

#include "Rendering\CameraManager.h"
#include "Rendering\Display.h"
#include "Rendering\DisplayManager.h"
#include "Rendering\LightManager.h"
#include "Rendering\MatrixStack.h"
#include "Rendering\Mesh.h"
#include "Resources\MeshResource.h"
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
		m_Projection(),
		m_RenderingMode(Rendering::RenderingMode::Forward),
		m_RenderTextureSize(0, 0),
		m_PostProcessShader(),
		m_ClearMode(ClearMode::ColorClear),
		m_ClearColor(Color(0.0f, 0.0f, 0.2f, 1.0f)),
		m_SkyboxMaterial(),
		m_DeferredRendererCompositor(0),

		m_Skybox(std::make_shared<Rendering::Skybox>()),
		m_RenderTexture(nullptr),
		m_RenderTextureDeferred(nullptr),
		m_DisplayRect(Rectangle(0, 0, 1, 1)),
		m_Quad(nullptr),
		m_PostProcessMaterial(nullptr),
		m_RegisterWithManager(registerWithManager)
	{
		m_Projection.setAspectRatio(Rendering::DisplayManager::getInstance().getDisplay(0)->getAspectRatio());
		m_Projection.recalculateProjectionMatrix();
	}

	Camera::~Camera()
	{
	}

	void Camera::clear()
	{
		// Clear the depth buffer
		Rendering::GraphicsAPI::clearDepthBuffer();

		// If the clear mode is set to ClearColor or SkyboxClear but no skybox is defined, clear it with the clear color
		if ((m_ClearMode == ColorClear) || (m_ClearMode == SkyboxClear && !m_SkyboxMaterial))
		{
			Rendering::GraphicsAPI::clearColorBuffer(m_ClearColor);
		}
		// Otherwise, if it passed the check before and is set to SkyboxClear then render the skybox
		else if (m_ClearMode == SkyboxClear)
		{
			auto skyboxMaterial = GetSkyboxMaterial();
			skyboxMaterial->bind();
			skyboxMaterial->getShader()->update(m_Projection.getRotationProjection(getGameObject()->getTransform()->getQuaternion()));
			m_Skybox->getMesh()->draw();
		}
		// NoClear will fall through and do nothing as we would expect it to do

	}

	SERIALIZE_FUNC_LOAD(archive, Camera)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Projection),
			SERIALIZE(m_RenderingMode),
			SERIALIZE(m_RenderTextureSize),
			SERIALIZE(m_PostProcessShader),
			SERIALIZE(m_ClearMode),
			SERIALIZE(m_ClearColor),
			SERIALIZE(m_SkyboxMaterial),
			SERIALIZE(m_DeferredRendererCompositor)
		);

		if (m_RenderTextureSize.x > 0 && m_RenderTextureSize.y > 0)
		{
			if (m_RenderingMode == Rendering::RenderingMode::Deferred)
			{
				m_RenderTextureDeferred = std::make_shared<Rendering::RenderTexture>(Rendering::RenderingMode::Deferred);
				m_RenderTextureDeferred->initializeTexture((int)m_RenderTextureSize.x, (int)m_RenderTextureSize.y);
			}
			m_RenderTexture = std::make_shared<Rendering::RenderTexture>(Rendering::RenderingMode::Forward);
			m_RenderTexture->initializeTexture((int)m_RenderTextureSize.x, (int)m_RenderTextureSize.y);
		}

		m_Projection.recalculateProjectionMatrix();
	}

	SERIALIZE_FUNC_SAVE(archive, Camera)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Projection),
			SERIALIZE(m_RenderingMode),
			SERIALIZE(m_RenderTextureSize),
			SERIALIZE(m_PostProcessShader),
			SERIALIZE(m_ClearMode),
			SERIALIZE(m_ClearColor),
			SERIALIZE(m_SkyboxMaterial),
			SERIALIZE(m_DeferredRendererCompositor)
		);
	}

	void Camera::init()
	{
		// If the flag is set to register for the manager, register this camera
		if (m_RegisterWithManager)
		{
			Rendering::CameraManager::getInstance().addCamera(std::static_pointer_cast<Camera>(shared_from_this()));
		}

		// Load the quad resource (used for deferred rendering compositing and post processing effects)
		auto quadResource = std::static_pointer_cast<Resources::MeshResource>(ObjectLibrary::getInstance().getResource("136a5d0f-51d7-4f3c-857c-0497de142a71"));
		if (quadResource != nullptr)
		{
			m_Quad = std::static_pointer_cast<Rendering::Mesh>(quadResource->getResourceObjectPointer());
		}

		// If this component is using post processing, bind the render texture to the post processing material
		if (m_PostProcessMaterial)
		{
			m_PostProcessMaterial->setTexture("RenderTex", m_RenderTexture);
		}
	}

	void Camera::preDestroy()
	{
		// If this has been registered with the manager, remove it from the manager before it is destroyed
		if (m_RegisterWithManager)
		{
			Rendering::CameraManager::getInstance().removeCamera(std::static_pointer_cast<Camera>(shared_from_this()));
		}
	}

	void Camera::renderScenesToActiveBuffer(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, int textureW, int textureH)
	{
		Rendering::LightManager::getInstance().uploadLightUniformBufferData(getGameObject()->getTransform());

		// Set the viewport to what is defined on this camera
		Rendering::GraphicsAPI::setViewport(std::static_pointer_cast<Camera>(shared_from_this()), textureW, textureH);

		// Set the aspect ratio to the texture size
		m_Projection.setAspectRatio(textureW, textureH);
		m_Projection.recalculateProjectionMatrix();

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
			root->render(std::make_shared<Rendering::MatrixStack>());
		}
	}

	void Camera::renderToAttachedRenderTexture(const std::vector<std::shared_ptr<Scenes::Scene>> scenes)
	{
		if (!m_RenderTexture)
		{
			return;
		}

		// Store the current framebuffer
		auto prevFramebufferId = Rendering::GraphicsAPI::getCurrentFramebufferID();

		auto rtex = m_RenderTextureDeferred == nullptr ? m_RenderTexture : m_RenderTextureDeferred;

		rtex->bindAsRenderTexture();

		// Render the scene
		auto scene = Scenes::SceneManager::getInstance().getActiveScene();
		Rendering::CameraManager::getInstance().setCurrentCamera(std::static_pointer_cast<Camera>(shared_from_this()));
		renderScenesToActiveBuffer({ scene }, rtex->getWidth(), rtex->getHeight());

		// Deferred rendering happens here
		if (m_RenderTextureDeferred)
		{
			assert(m_DeferredRendererCompositor);

			// Render it to the render texture
			m_RenderTexture->bindAsRenderTexture();

			Rendering::GraphicsAPI::clearDepthBuffer();
			Rendering::GraphicsAPI::clearColorBuffer({ 0.0f, 0.0f, 0.0f, 1.0f });
			glDisable(GL_DEPTH_TEST);

			// Set the identity matrices so that the quad renders to the entire render area
			auto deferredCompositorShader = GetDeferredCompositorShader();
			setIdentityMatricies(deferredCompositorShader);

			// Render the full-buffer quad
			m_RenderTextureDeferred->bindDeferredTextures(deferredCompositorShader);
			Rendering::LightManager::getInstance().bindLightsToShader(deferredCompositorShader);
			deferredCompositorShader->renderMesh(m_Quad, nullptr);
		}

		// Postprocessing happens here
		if (m_PostProcessMaterial != nullptr)
		{
			glDisable(GL_DEPTH_TEST);

			m_PostProcessMaterial->bind();

			// Load the shader with matricies that will transform the quad to take up the entire buffer
			auto postProcessShader = m_PostProcessMaterial->getShader();
			setIdentityMatricies(postProcessShader);

			postProcessShader->setInt("Width", m_RenderTexture->getWidth());
			postProcessShader->setInt("Height", m_RenderTexture->getHeight());

			// Render the full-buffer quad
			postProcessShader->renderMesh(m_Quad, m_RenderTexture);
		}

		// Rebind the old framebuffer
		Rendering::GraphicsAPI::bindFramebuffer(prevFramebufferId);
	}

	void Camera::resize(int const& width, int const& height)
	{
		m_Projection.setAspectRatio(width, height);
		m_Projection.recalculateProjectionMatrix();
	}

	// TODO: Replace with new function Camera::getDisplaySize()
	float Camera::getDisplayHeight()
	{
		return m_RenderTexture != nullptr ? (float)m_RenderTexture->getHeight() : (float)Rendering::DisplayManager::getInstance().getDisplay(0)->getActualSize().y;
	}

	// TODO: Replace with new function Camera::getDisplaySize()
	float Camera::getDisplayWidth()
	{
		return m_RenderTexture != nullptr ? (float)m_RenderTexture->getWidth() : (float)Rendering::DisplayManager::getInstance().getDisplay(0)->getActualSize().x;
	}

	void Camera::setDisplayRect(float const& x, float const& y, float const& w, float const& h)
	{
		m_DisplayRect.setX(x);
		m_DisplayRect.setY(x);
		m_DisplayRect.setWidth(x);
		m_DisplayRect.setHeight(x);
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

	void Camera::setProjectionMode(Rendering::ProjectionMode const& mode)
	{
		m_Projection.setProjectionMode(mode);
		m_Projection.recalculateProjectionMatrix();
	}

}