#pragma once
#include <vector>
#include "Display.h"

class DisplayManager
{
public:
	static DisplayManager& getInstance()
	{
		static DisplayManager instance;
		return instance;
	}

	void addDisplay(Display* display) { m_displays.push_back(display); }

	Display* getDisplay(int index) {
		if (index >= m_displays.size())
		{
			return nullptr;
		}
		return m_displays[index];
	}

	void operator=(DisplayManager const&) = delete;
private:
	DisplayManager() {}
	~DisplayManager() {}

	DisplayManager(DisplayManager const&);

	std::vector<Display*> m_displays;
};

