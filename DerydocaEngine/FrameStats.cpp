#include "FrameStats.h"

namespace DerydocaEngine::Components
{

	void FrameStats::postInit()
	{
		m_textRenderer = getComponent<DerydocaEngine::Components::TextRenderer>();
	}

	void FrameStats::update(float const& deltaTime)
	{
		float fps = 1.0f / deltaTime;
		std::ostringstream s;
		s << "FPS: " << fps;
		m_textRenderer->setText(s.str());
	}

}