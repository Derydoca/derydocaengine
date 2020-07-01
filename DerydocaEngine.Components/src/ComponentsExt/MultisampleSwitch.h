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
		bool isMultisampleOn() { return m_EnableMultisample; }

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent);
	private:
		Input::Keyboard * m_Keyboard;
		bool m_EnableMultisample;
		bool m_MSAASupported;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::MultisampleSwitch);
