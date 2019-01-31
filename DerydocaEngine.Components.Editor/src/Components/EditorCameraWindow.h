#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"
#include "Rendering\RenderTexture.h"
#include "Components\Camera.h"
#include "Input\InputManager.h"

namespace DerydocaEngine::Components
{
	class EditorCameraWindow : public EditorWindowComponent, SelfRegister<EditorCameraWindow>
	{
	public:
		GENINSTANCE(EditorCameraWindow);

		EditorCameraWindow();
		~EditorCameraWindow();

		virtual void init();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
		virtual void update(const float deltaTime);

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Editor Camera"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

	private:
		bool m_isActive;
		int m_windowWidth;
		int m_windowHeight;

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