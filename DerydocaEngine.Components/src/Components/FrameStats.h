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
		virtual void update(const float deltaTime);

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent);
	private:
		std::shared_ptr<TextRenderer> m_textRenderer;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::FrameStats);
