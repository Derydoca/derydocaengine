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

		float& getGamma() { return m_gamma; }
	private:
		float m_gamma;
		std::shared_ptr<Components::Camera> m_postProcessCamera;

		void updateShader();
	};

}
