#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"

namespace DerydocaEngine::Components
{
	class EditorCameraWindow : public EditorWindowComponent, SelfRegister<EditorCameraWindow>
	{
	public:
		GENINSTANCE(EditorCameraWindow);

		EditorCameraWindow();
		~EditorCameraWindow();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Editor Camera"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

	private:
		std::weak_ptr<Resources::ResourceTreeNode> m_resourceNode;
		std::shared_ptr<Editor::SelectionGroup> m_selectionGroup;
	};
}