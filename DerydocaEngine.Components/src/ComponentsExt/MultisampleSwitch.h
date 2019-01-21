#pragma once
#include "Components\GameComponent.h"
#include "Input\Keyboard.h"

namespace DerydocaEngine::Ext
{

	class MultisampleSwitch : public Components::GameComponent, Components::SelfRegister<MultisampleSwitch>
	{
	public:
		GENINSTANCE(MultisampleSwitch);

		MultisampleSwitch();
		~MultisampleSwitch();

		virtual void init();
		virtual void update(const float deltaTime);

		void setEnableMultisample(bool const& enable);
		bool isMultisampleOn() { return m_enableMultisample; }
	private:
		Input::Keyboard * m_keyboard;
		bool m_enableMultisample;
		bool m_msaaSupported;
	};

}