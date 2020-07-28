#pragma once
#include "Editor\SelectionManager.h"
#include "Components\SceneViewerWindow.h"

namespace DerydocaEngine::Components
{
	class EditorCameraWindow : public SceneViewerWindow, SelfRegister<EditorCameraWindow>
	{
	public:
		GENINSTANCE(EditorCameraWindow);

		EditorCameraWindow();
		~EditorCameraWindow();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Editor Camera"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }
		virtual glm::vec2 getViewPadding() override;
		virtual void renderToActiveBuffer() override;

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::SceneViewerWindow);
	private:
		bool m_ShowPropertiesPanel;

	};
}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::EditorCameraWindow, 0);
