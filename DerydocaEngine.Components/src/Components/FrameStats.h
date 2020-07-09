#pragma once
#include "Components\GameComponent.h"
#include "TextRenderer.h"

namespace DerydocaEngine::Components
{

	class FrameStats : public GameComponent, SelfRegister<FrameStats>
	{
	public:
		GENINSTANCE(FrameStats);
		SERIALIZE_FUNC_DEFINITIONS;
		FrameStats();
		~FrameStats();

		virtual void postInit();
		virtual void update(const float deltaTime);
	private:
		std::shared_ptr<TextRenderer> m_textRenderer;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::FrameStats, 0);
