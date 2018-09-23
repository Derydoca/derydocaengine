#pragma once
#include <vector>

namespace DerydocaEngine::Rendering {
	class Display;
}

namespace DerydocaEngine::Rendering
{

	class DisplayManager
	{
	public:
		static DisplayManager& getInstance()
		{
			static DisplayManager instance;
			return instance;
		}

		void addDisplay(Display* const& display);

		Display* getDisplay(size_t const& index) const;

		void operator=(DisplayManager const&) = delete;
	private:
		DisplayManager();
		~DisplayManager();

		DisplayManager(DisplayManager const&);

		std::vector<Display*> m_displays;
	};

}
