#include "EngineComponentsPch.h"
#include "Components/ScreenshotUtil.h"

#include <chrono>
#include <ctime>
#include <GL/glew.h>
#include "Rendering/Display.h"
#include "Rendering/DisplayManager.h"
#include "Input/InputManager.h"
#include "sdl2/SDL.h"

namespace DerydocaEngine::Components
{
	ScreenshotUtil::ScreenshotUtil() :
		m_Keyboard(),
		m_Display()
	{
		m_Keyboard = Input::InputManager::getInstance().getKeyboard();
		m_Display = Rendering::DisplayManager::getInstance().getDisplay(0);
	}

	ScreenshotUtil::~ScreenshotUtil()
	{
	}

	void ScreenshotUtil::saveScreenshot()
	{
		// Build a file name with a unique time stamp
		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(p);

		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::stringstream ss;
		ss << "c:/test/ss_" << now << ".png";
		std::string filePath = ss.str();

		// Get the screen buffer's content
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		int2 size = m_Display->getActualSize();
		GLubyte* data = new GLubyte[size.x * size.y * 3];
		glReadPixels(0, 0, size.x, size.y, GL_RGB, GL_UNSIGNED_BYTE, data);

		// Write it to disk
		stbi_flip_vertically_on_write(1);
		int result = stbi_write_png(filePath.c_str(), size.x, size.y, 3, data, size.x * 3);
		delete[] data;

		// Give some feedback
		if (result == 0)
		{
			D_LOG_ERROR("Unable to save screenshot.");
		}
		else
		{
			D_LOG_INFO("Screenshot saved!");
		}
	}

	void ScreenshotUtil::postRender()
	{
		if (m_Keyboard->isKeyDownFrame(SDLK_p))
		{
			saveScreenshot();
		}
	}

	SERIALIZE_FUNC_LOAD(archive, ScreenshotUtil)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent));
	}

	SERIALIZE_FUNC_SAVE(archive, ScreenshotUtil)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent));
	}

}
