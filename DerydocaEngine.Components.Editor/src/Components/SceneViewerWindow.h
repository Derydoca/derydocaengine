#pragma once
#include "Components\EditorWindowComponent.h"
#include "Rendering\RenderTexture.h"
#include "Components\Camera.h"
#include "Input\InputManager.h"

namespace DerydocaEngine::Components
{

	class SceneViewerWindow : public EditorWindowComponent
	{
	public:
		SceneViewerWindow();
		virtual ~SceneViewerWindow();

		virtual void init() override;
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) override;
		virtual void update(const float deltaTime) override;

	protected:
		void renderViewToWindow();
		void updateDisplayProperties();

		virtual glm::vec2 getViewPadding() = 0;
		virtual void renderToActiveBuffer() = 0;

		bool isActive() { return m_isActive; }
		int getDisplayWidth() { return m_displayWidth; }
		int getDisplayHeight() { return m_displayHeight; }
		std::shared_ptr<Components::Camera> getCamera() { return m_camera; }

	private:
		// Display variables
		bool m_isActive;
		float m_displayWidth;
		float m_displayHeight;

		// Mouse rotation vars
		Input::Mouse* m_mouse;
		float m_mouseSensitivityX;
		float m_mouseSensitivityY;
		float m_minXRot;
		float m_maxXRot;
		glm::vec2 m_eulerRot;

		// Keyboard movement vars
		Input::Keyboard* m_keyboard;
		float m_moveSpeed;

		std::shared_ptr<Rendering::RenderTexture> m_renderTexture;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<Components::Transform> m_cameraTransform;

	};

}
