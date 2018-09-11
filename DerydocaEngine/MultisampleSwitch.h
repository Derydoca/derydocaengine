#pragma once
#include "GameComponent.h"
#include "Keyboard.h"

namespace DerydocaEngine::Ext
{

	class MultisampleSwitch : public GameComponent
	{
	public:
		GENINSTANCE(MultisampleSwitch);

		MultisampleSwitch();
		~MultisampleSwitch();

		virtual void init();
		virtual void update(float const& deltaTime);

		void setEnableMultisample(bool const& enable);
	private:
		Keyboard * m_keyboard;
		bool m_enableMultisample;
		bool m_msaaSupported;
	};

}