#pragma once
#include "Components\GameComponent.h"
#include "Rendering\RenderTexture.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Ext
{

	class GammaCorrectionFilter : public Components::GameComponent, Components::SelfRegister<GammaCorrectionFilter>
	{
	public:
		GENINSTANCE(GammaCorrectionFilter);

		GammaCorrectionFilter();
		~GammaCorrectionFilter();

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void update(const float deltaTime);

		float& getGamma() { return m_Gamma; }

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_Gamma)
		);
	private:
		float m_Gamma;

		std::shared_ptr<Components::Camera> m_PostProcessCamera;

		void updateShader();
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::GammaCorrectionFilter, 0);
