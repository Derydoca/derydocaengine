#include "FrameStats.h"

FrameStats::FrameStats()
{
}

FrameStats::~FrameStats()
{
}

void FrameStats::postInit()
{
	m_textRenderer = getComponent<TextRenderer>();
}

void FrameStats::update(float deltaTime)
{
	float fps = 1.0f / deltaTime;
	ostringstream s;
	s << "FPS: " << fps;
	m_textRenderer->setText(s.str());
}
