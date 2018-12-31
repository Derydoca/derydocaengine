#include "EngineComponentsPch.h"
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

		ImGui::Begin(m_windowName.c_str(), &m_isOpen, getWindowFlags());
		renderWindow();
		ImGui::End();
	}

}
