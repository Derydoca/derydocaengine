#include "EditorComponentsPch.h"
#include "ImGuiDemoWindow.h"

namespace DerydocaEngine::Components
{
	ImGuiDemoWindow::ImGuiDemoWindow() :
		m_Open(true)
	{
	}

	void ImGuiDemoWindow::update(const float deltaTime)
	{
		ImGui::ShowDemoWindow(&m_Open);
		if (!m_Open)
		{
			destroy(getGameObject());
		}
	}

}
