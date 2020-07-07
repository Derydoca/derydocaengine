#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Components
{

	class EditorWindowComponent : public GameComponent
	{
	public:
		EditorWindowComponent();
		virtual ~EditorWindowComponent();

		virtual glm::ivec2 getWindowInitialSize() { return {0.0f, 0.0f}; }
		virtual ImGuiWindowFlags getWindowFlags() = 0;
		virtual std::string getWindowTitle() = 0;
		virtual void renderWindow() = 0;

		virtual void postInit();
		virtual void renderEditorGUI();
		void close() { m_IsOpen = false; }

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent);
	protected:
		bool m_IsOpen;
		std::string m_WindowName;
	};

}