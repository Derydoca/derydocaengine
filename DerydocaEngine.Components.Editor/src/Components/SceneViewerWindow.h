#pragma once
#include "Components\Camera.h"
#include "Components\EditorWindowComponent.h"
#include "Input\InputManager.h"
#include "Rendering\RenderTexture.h"
#include "SceneCameraInput\SceneCameraInputStrategy.h"

namespace DerydocaEngine::Components
{

	class SceneViewerWindow : public EditorWindowComponent
	{
	public:
		SceneViewerWindow(std::shared_ptr<SceneCameraInput::SceneCameraInputStrategy> inputStrategy);
		virtual ~SceneViewerWindow();

		virtual void init() override;
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) override;
		virtual void update(const float deltaTime) override;

	protected:
		void renderViewToWindow();
		void updateDisplayProperties();

		virtual glm::vec2 getViewPadding() = 0;
		virtual void renderToActiveBuffer() = 0;

		bool isActive() const { return m_isActive; }
		bool isViewportHovered() const { return m_isViewportHovered; }
		float getDisplayWidth() const { return m_displayWidth; }
		float getDisplayHeight() const { return m_displayHeight; }
		std::shared_ptr<Components::Camera> getCamera() { return m_camera; }

	protected:
		std::shared_ptr<SceneCameraInput::SceneCameraInputStrategy> m_sceneCameraInputStrategy;

	private:
		// Display variables
		bool m_isActive;
		bool m_isViewportHovered;
		float m_displayWidth;
		float m_displayHeight;

		std::shared_ptr<Rendering::RenderTexture> m_renderTexture;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<Components::Transform> m_cameraTransform;

	};

}
