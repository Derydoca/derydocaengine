#include "EditorComponentsPch.h"
#include "GameComponentInspectorWindow.h"

namespace DerydocaEngine::Components
{
	GameComponentInspectorWindow::GameComponentInspectorWindow()
	{
		m_selectionGroup = Editor::SelectionManager::getInstance().getPrimarySelectionGroup();
	}

	GameComponentInspectorWindow::~GameComponentInspectorWindow()
	{
	}

	void GameComponentInspectorWindow::renderWindow()
	{
		auto selection = m_selectionGroup->getSelection();

		ImGui::Text(std::to_string(selection.size()).c_str());
		ImGui::Text("The GameComponent inspector window is not yet implemented.");
	}

}
