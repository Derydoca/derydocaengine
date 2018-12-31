#pragma once
#include "GameComponent.h"

namespace DerydocaEngine::Components
{

	class EditorWindowComponent : public GameComponent
	{
	public:
		EditorWindowComponent();
		virtual ~EditorWindowComponent();

		virtual ImGuiWindowFlags getWindowFlags() = 0;
		virtual std::string getWindowTitle() = 0;
		virtual void renderWindow() = 0;

		virtual void postInit();
		virtual void renderEditorGUI();
		void close() { m_isOpen = false; }
	protected:
		bool m_isOpen;
		std::string m_windowName;
	};

}