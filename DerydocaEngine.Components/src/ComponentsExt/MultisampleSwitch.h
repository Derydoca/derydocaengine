#pragma once
#include "Components\GameComponent.h"
#include "Input\Keyboard.h"

namespace DerydocaEngine::Ext
{

	class MultisampleSwitch : public Components::GameComponent, Components::SelfRegister<MultisampleSwitch>
	{
	public:
		GENINSTANCE(MultisampleSwitch);
		SERIALIZE_FUNC_DEFINITIONS;

		MultisampleSwitch();
		~MultisampleSwitch();

		virtual void init();
		virtual void update(const float deltaTime);

		void setEnableMultisample(bool const& enable);
		bool isMultisampleOn() { return m_EnableMultisample; }

	private:
		Input::Keyboard * m_Keyboard;
		bool m_EnableMultisample;
		bool m_MSAASupported;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::MultisampleSwitch, 0);
