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

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent);
	private:
		bool m_Open;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::ImGuiDemoWindow);
