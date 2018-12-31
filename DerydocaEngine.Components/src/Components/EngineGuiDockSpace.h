#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Components
{
	class EngineGuiDockSpace : public GameComponent, SelfRegister<EngineGuiDockSpace>
	{
	public:
		GENINSTANCE(EngineGuiDockSpace);

		EngineGuiDockSpace();
		~EngineGuiDockSpace();

		virtual void renderEditorGUI();
	};
}