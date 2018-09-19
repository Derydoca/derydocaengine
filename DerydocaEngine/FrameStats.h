#pragma once
#include "GameComponent.h"
#include "TextRenderer.h"

namespace DerydocaEngine::Components
{

	class FrameStats : public GameComponent
	{
	public:
		GENINSTANCE(FrameStats);
		FrameStats() {}
		~FrameStats() {}

		virtual void postInit();
		virtual void update(float const& deltaTime);
	private:
		TextRenderer * m_textRenderer;
	};

}
