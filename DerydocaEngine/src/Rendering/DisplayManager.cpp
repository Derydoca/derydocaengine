#include "EnginePch.h"
#include "Rendering\DisplayManager.h"

#include "Rendering\Display.h"

namespace DerydocaEngine::Rendering
{

	void DisplayManager::addDisplay(Display* const& display)
	{
		m_displays.push_back(display);
	}

	Display* DisplayManager::getDisplay(size_t const& index) const
	{
		if (index >= m_displays.size())
		{
			return nullptr;
		}
		return m_displays[index];
	}

	DisplayManager::DisplayManager()
	{
	}

	DisplayManager::~DisplayManager()
	{
	}

}
