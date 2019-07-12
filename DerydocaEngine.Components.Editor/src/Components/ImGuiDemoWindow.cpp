#include "EditorComponentsPch.h"
#include "ImGuiDemoWindow.h"

namespace DerydocaEngine::Components
{
	ImGuiDemoWindow::ImGuiDemoWindow() :
		m_open(true)
	{
	}

	void ImGuiDemoWindow::update(const float deltaTime)
	{
		ImGui::ShowDemoWindow(&m_open);
		if (!m_open)
		{
			destroy(getGameObject());
		}
	}

}
