#include "EditorComponentsPch.h"
#include "EditorWindowComponent.h"

namespace DerydocaEngine::Components
{
	EditorWindowComponent::EditorWindowComponent() :
		m_IsOpen(true),
		m_WindowName()
	{
	}

	EditorWindowComponent::~EditorWindowComponent()
	{
	}

	void EditorWindowComponent::postInit()
	{
		m_WindowName = getWindowTitle() + "##" + boost::lexical_cast<std::string>(getGameObject()->getId());
	}

	void EditorWindowComponent::renderEditorGUI()
	{
		if (!m_IsOpen)
		{
			destroy(getGameObject());
		}

		ImVec2 size = {20.0f, 20.0f};
		{
			auto initSize = getWindowInitialSize();
			size.x = static_cast<float>(initSize.x);
			size.y = static_cast<float>(initSize.y);
		}
		if (size.x > 0 && size.y > 0)
		{
			ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		}
		ImGui::Begin(m_WindowName.c_str(), &m_IsOpen, getWindowFlags());
		renderWindow();
		ImGui::End();
	}

}
