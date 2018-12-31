#pragma once
#include "Components\GameComponent.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Ext
{

	class GaussianBlurFilter : public Components::GameComponent, Components::SelfRegister<GaussianBlurFilter>
	{
	public:
		GENINSTANCE(GaussianBlurFilter);

		GaussianBlurFilter() {}
		~GaussianBlurFilter() {}

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void update(const float deltaTime);
	private:
		std::shared_ptr<Components::Camera> m_postProcessCamera;
		float m_weights[5];

		float gauss(float const& x, float const& sigma2);
		void updateShader();
	};

}
