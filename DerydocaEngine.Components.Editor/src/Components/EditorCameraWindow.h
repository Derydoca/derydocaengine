#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"
#include "Rendering\RenderTexture.h"

namespace DerydocaEngine::Components
{
	class EditorCameraWindow : public EditorWindowComponent, SelfRegister<EditorCameraWindow>
	{
	public:
		GENINSTANCE(EditorCameraWindow);

		EditorCameraWindow();
		~EditorCameraWindow();

		virtual void postRender();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Editor Camera"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

	private:
		std::shared_ptr<Rendering::RenderTexture> m_renderTexture;

	};
}