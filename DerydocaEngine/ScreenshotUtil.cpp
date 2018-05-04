#include "ScreenshotUtil.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "stb_image.h"

using namespace boost::posix_time;
using namespace std;

string convertTimeToString(ptime now)
{
	static std::locale loc(std::wcout.getloc(), new time_facet("%Y%m%d%H%M%S"));
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

		int w = m_display->getWidth();
		int h = m_display->getHeight();

		cout << "SCREENSHOTS ARE NOT SUPPORTED AT THIS TIME" << endl;
		/*GLubyte* data = malloc(w * h * sizeof(Uint32));
		glReadPixels(0, 0, , GL_RGB, GL_UNSIGNED_INT, )
		stbi__create_png_image();

		int result = SOIL_save_screenshot(file.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, m_display->getWidth(), m_display->getHeight());
		if (result == 0) {
			printf("Unable to save screenshot.\n");
		}
		else {
			printf("Screenshot saved!\n");
		}*/
	}
}
