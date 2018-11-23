#pragma once
#include "Components\GameComponent.h"
#include "Keyboard.h"

namespace DerydocaEngine::Ext
{

	class MultisampleSwitch : public Components::GameComponent, Components::SelfRegister<MultisampleSwitch>
	{
	public:
		GENINSTANCE(MultisampleSwitch);

		MultisampleSwitch();
		~MultisampleSwitch();

		virtual void init();
		virtual void update(float const& deltaTime);

		void setEnableMultisample(bool const& enable);
	private:
		Input::Keyboard * m_keyboard;
		bool m_enableMultisample;
		bool m_msaaSupported;
	};

}