#include "EditorComponentsPch.h"
#include "EditorWindowComponent.h"

namespace DerydocaEngine::Components
{
	EditorWindowComponent::EditorWindowComponent() :
		m_isOpen(true),
		m_windowName()
	{
	}

	EditorWindowComponent::~EditorWindowComponent()
	{
	}

	void EditorWindowComponent::postInit()
	{
		m_windowName = getWindowTitle() + "##" + boost::lexical_cast<std::string>(getGameObject()->getId());
	}

	void EditorWindowComponent::renderEditorGUI()
	{
		if (!m_isOpen)
		{
			return;
		}

		ImVec2 size = {20.0f, 20.0f};
		{
			auto initSize = getWindowInitialSize();
			size.x = initSize.x;
			size.y = initSize.y;
		}
		if (size.x > 0 && size.y > 0)
		{
			ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		}
		ImGui::Begin(m_windowName.c_str(), &m_isOpen, getWindowFlags());
		renderWindow();
		ImGui::End();
	}

}
