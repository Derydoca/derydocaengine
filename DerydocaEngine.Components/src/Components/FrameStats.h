#pragma once
#include "Components\GameComponent.h"
#include "TextRenderer.h"

namespace DerydocaEngine::Components
{

	class FrameStats : public GameComponent, SelfRegister<FrameStats>
	{
	public:
		GENINSTANCE(FrameStats);
		FrameStats();
		~FrameStats();

		virtual void postInit();
		virtual void update(float const& deltaTime);
	private:
		std::shared_ptr<TextRenderer> m_textRenderer;
	};

}
