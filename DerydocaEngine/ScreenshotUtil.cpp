#include "ScreenshotUtil.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::posix_time;
using namespace std;

string convertTimeToString(ptime now)
{
	static std::locale loc(std::wcout.getloc(), new wtime_facet(L"%Y%m%d%H%M%S"));
	std::basic_stringstream<char> wss;
	wss.imbue(loc);
	wss << now;
	return wss.str();
}

ScreenshotUtil::ScreenshotUtil()
{
}

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
		
		ptime now = second_clock::universal_time();
		string timestamp = convertTimeToString(now);

		//std::tm tm = *std::localtime(&t);
		//char s[26];
		//strftime(s, sizeof s, "%Y%m%d%H%M%S", &tm);
		std::string file = "c:/test/ss_" + timestamp + ".bmp";

		// Save the file
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 800, 600);
		int result = SOIL_save_screenshot(file.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, m_display->getWidth(), m_display->getHeight());
		if (result == 0) {
			printf("Unable to save screenshot.\n");
		}
		else {
			printf("Screenshot saved!\n");
		}
	}
}
