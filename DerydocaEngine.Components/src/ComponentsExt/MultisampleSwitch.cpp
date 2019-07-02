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
			LOG_INFO("MSAA: Buffers = {}, samples = {}", bufs, samples);
		}
		else
		{
			LOG_WARN("MSAA is not supported.");
		}
	}

	void MultisampleSwitch::update(const float deltaTime)
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

		LOG_INFO("Multisampling Enabled: {}", m_enableMultisample);
	}

}
