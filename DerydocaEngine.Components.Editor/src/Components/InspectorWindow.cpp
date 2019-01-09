#include "EditorComponentsPch.h"
#include "InspectorWindow.h"
#include "Resources\MeshResource.h"
#include "Resources\ShaderResource.h"
#include "Resources\LevelResource.h"
#include "Object.h"
#include "Scenes\SceneManager.h"

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
			return;
		}

		auto firstSelection = selection[0];
		
		auto typeId = firstSelection->getTypeId();

		if (typeId == DerydocaEngine::getTypeId<Resources::MeshResource>())
		{
			ImGui::Text("This is a mesh.");
		}
		else if (typeId == DerydocaEngine::getTypeId<Resources::ShaderResource>())
		{
			ImGui::Text("This is a shader.");
		}
		else if (typeId == DerydocaEngine::getTypeId<Resources::LevelResource>())
		{
			ImGui::Text("This is a level.");
			if (ImGui::Button("Load"))
			{
				Scenes::SceneManager::getInstance().loadScene(std::static_pointer_cast<Resources::LevelResource>(firstSelection));
			}
		}
		else
		{
			ImGui::Text("There is no inspector for this object type.");
		}
	}

}
