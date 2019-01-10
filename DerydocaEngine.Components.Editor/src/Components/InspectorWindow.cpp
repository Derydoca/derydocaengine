#include "EditorComponentsPch.h"
#include "InspectorWindow.h"
#include "Editor\Inspector\InspectorRendererFactory.h"

namespace DerydocaEngine::Components
{
	InspectorWindow::InspectorWindow()
	{
		m_selectionGroup = Editor::SelectionManager::getInstance().getPrimarySelectionGroup();
	}

	InspectorWindow::~InspectorWindow()
	{
	}

	void InspectorWindow::renderWindow()
	{
		auto selection = m_selectionGroup->getSelection();

		if (selection.size() <= 0)
		{
			ImGui::Text("No objects are selected.");
			return;
		}

		Editor::Inspector::InspectorRendererFactory::getInstance().renderInspector(selection[0]);
	}

}
