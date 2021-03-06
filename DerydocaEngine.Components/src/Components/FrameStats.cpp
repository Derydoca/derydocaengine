#include "EngineComponentsPch.h"
#include "FrameStats.h"

namespace DerydocaEngine::Components
{

	FrameStats::FrameStats() :
		m_textRenderer()
	{
	}

	FrameStats::~FrameStats()
	{
	}

	void FrameStats::postInit()
	{
		m_textRenderer = getComponentInChildren<TextRenderer>();
	}

	void FrameStats::update(const float deltaTime)
	{
		float fps = 1.0f / deltaTime;
		std::ostringstream s;
		s << "FPS: " << fps;
		m_textRenderer->setText(s.str());
	}

	SERIALIZE_FUNC_LOAD(archive, FrameStats)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent));
	}

	SERIALIZE_FUNC_SAVE(archive, FrameStats)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent));
	}

}