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
        m_WindowName = getWindowTitle() + "##" + getGameObject()->getId().to_string();
    }

    void EditorWindowComponent::renderEditorGUI()
    {
        if (!m_IsOpen)
        {
            destroy(getGameObject());
        }

        ImVec2 size = { 20.0f, 20.0f };
        {
            auto initSize = getWindowInitialSize();
            size.x = static_cast<float>(initSize.x);
            size.y = static_cast<float>(initSize.y);
        }
        if (size.x > 0 && size.y > 0)
        {
            ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        }
        auto windowPadding = getWindowPadding();
        bool hasCustomWindowPadding = windowPadding != glm::vec2(-1, -1);
        if (hasCustomWindowPadding)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { windowPadding.x, windowPadding.y });
        }
        ImGui::Begin(m_WindowName.c_str(), &m_IsOpen, getWindowFlags());
        renderWindow();
        ImGui::End();
        if (hasCustomWindowPadding)
        {
            ImGui::PopStyleVar();
        }
    }

}
