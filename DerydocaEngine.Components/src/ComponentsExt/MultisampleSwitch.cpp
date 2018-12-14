#include "EngineComponentsPch.h"
#include "MultisampleSwitch.h"

#include <iostream>
#include "GL\glew.h"
#include "Input\InputManager.h"
#include "sdl2\SDL.h"

namespace DerydocaEngine::Ext
{

	MultisampleSwitch::MultisampleSwitch()
	{
		m_keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	MultisampleSwitch::~MultisampleSwitch()
	{
	}

	void MultisampleSwitch::init()
	{
		GLint bufs = 0;
		GLint samples = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
		glGetIntegerv(GL_SAMPLES, &samples);

		m_msaaSupported = bufs > 0 && samples > 0;

		if (m_msaaSupported)
		{
			std::cout << "MSAA: Buffers = " << bufs << ", samples = " << samples << "\n";
		}
		else
		{
			std::cout << "MSAA is not supported!\n";
		}
	}

	void MultisampleSwitch::update(float const& deltaTime)
	{
		if (m_keyboard->isKeyDownFrame(SDLK_l))
		{
			setEnableMultisample(!m_enableMultisample);
		}
	}

	void MultisampleSwitch::setEnableMultisample(bool const& enable)
	{
		if (!m_msaaSupported)
		{
			return;
		}

		m_enableMultisample = enable;
		if (m_enableMultisample)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}

		std::cout << "Multisampling Enabled: " << m_enableMultisample << "\n";
	}

}
