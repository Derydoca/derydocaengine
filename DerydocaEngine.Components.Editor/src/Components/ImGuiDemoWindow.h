#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Components
{

	class ImGuiDemoWindow : public GameComponent, SelfRegister<ImGuiDemoWindow>
	{
	public:
		GENINSTANCE(ImGuiDemoWindow);

		ImGuiDemoWindow();

		virtual void update(const float deltaTime);
	private:
		bool m_open;
	};

}