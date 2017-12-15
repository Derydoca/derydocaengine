#include "ScreenshotUtil.h"



ScreenshotUtil::ScreenshotUtil(Display* display, Keyboard* keyboard) :
	m_display(display),
	m_keyboard(keyboard)
{
}


ScreenshotUtil::~ScreenshotUtil()
{
}

void ScreenshotUtil::postRender()
{
	if(m_keyboard->isKeyDownFrame(SDLK_p)) {
		// Build a file name with a unique time stamp
		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(p);
		std::tm tm = *std::localtime(&t);
		char s[26];
		strftime(s, sizeof s, "%Y%m%d%H%M%S", &tm);
		
		std::string file = "c:/test/ss_" + std::string(s) + ".bmp";

		// Save the file
		int result = SOIL_save_screenshot(file.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, m_display->getWidth(), m_display->getHeight());
		if (result == 0) {
			printf("Unable to save screenshot. Error code: " + result);
		}
		else {
			printf("Screenshot saved!");
		}
	}
}
