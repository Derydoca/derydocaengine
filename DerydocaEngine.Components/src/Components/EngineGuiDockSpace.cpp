#include "EngineComponentsPch.h"
#include "EngineGuiDockSpace.h"

namespace DerydocaEngine::Components
{

	EngineGuiDockSpace::EngineGuiDockSpace()
	{
	}

	EngineGuiDockSpace::~EngineGuiDockSpace()
	{
	}

	void EngineGuiDockSpace::renderEditorGUI()
	{
		ImGui::DockSpace(1, ImVec2(300, 300));
	}

}