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
		m_Keyboard = Input::InputManager::getInstance().getKeyboard();
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

		m_MSAASupported = bufs > 0 && samples > 0;

		if (m_MSAASupported)
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
		if (m_Keyboard->isKeyDownFrame(SDLK_l))
		{
			setEnableMultisample(!m_EnableMultisample);
		}
	}

	void MultisampleSwitch::setEnableMultisample(bool const& enable)
	{
		if (!m_MSAASupported)
		{
			return;
		}

		m_EnableMultisample = enable;
		if (m_EnableMultisample)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}

		LOG_INFO("Multisampling Enabled: {}", m_EnableMultisample);
	}

}
