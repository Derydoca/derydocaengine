#include "EngineComponentsPch.h"
#include "Components\ScreenshotUtil.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>
#include <ctime>
#include <GL/glew.h>
#include "Rendering\Display.h"
#include "Rendering\DisplayManager.h"
#include "Input\InputManager.h"
#include "sdl2\SDL.h"

namespace t = boost::posix_time;

namespace DerydocaEngine::Components
{

	std::string convertTimeToString(const t::ptime& now)
	{
		static std::locale loc(std::wcout.getloc(), new t::time_facet("%Y%m%d%H%M%S"));
		std::basic_stringstream<char> wss;
		wss.imbue(loc);
		wss << now;
		return wss.str();
	}

	ScreenshotUtil::ScreenshotUtil() :
		m_keyboard(),
		m_display()
	{
		m_keyboard = Input::InputManager::getInstance().getKeyboard();
		m_display = Rendering::DisplayManager::getInstance().getDisplay(0);
	}

	ScreenshotUtil::~ScreenshotUtil()
	{
	}

	void ScreenshotUtil::saveScreenshot()
	{
		// Build a file name with a unique time stamp
		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(p);

		t::ptime now = t::second_clock::universal_time();
		std::string timestamp = convertTimeToString(now);

		std::string file = "c:/test/ss_" + timestamp + ".png";

		// Get the screen buffer's content
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		int2 size = m_display->getActualSize();
		GLubyte* data = new GLubyte[size.x * size.y * 3];
		glReadPixels(0, 0, size.x, size.y, GL_RGB, GL_UNSIGNED_BYTE, data);

		// Write it to disk
		stbi_flip_vertically_on_write(1);
		int result = stbi_write_png(file.c_str(), size.x, size.y, 3, data, size.x * 3);
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
		if (m_keyboard->isKeyDownFrame(SDLK_p))
		{
			saveScreenshot();
		}
	}

}
