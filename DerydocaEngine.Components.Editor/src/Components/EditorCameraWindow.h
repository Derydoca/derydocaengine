#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"
#include "Rendering\RenderTexture.h"
#include "Components\Camera.h"

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

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Editor Camera"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

	private:
		int m_windowWidth;
		int m_windowHeight;
		std::shared_ptr<Rendering::RenderTexture> m_renderTexture;
		std::shared_ptr<Camera> m_camera;

	};
}