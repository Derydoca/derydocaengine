#pragma once
#include "Components\GameComponent.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Ext
{

	class GaussianBlurFilter : public Components::GameComponent, Components::SelfRegister<GaussianBlurFilter>
	{
	public:
		GENINSTANCE(GaussianBlurFilter);
		SERIALIZE_FUNC_DEFINITIONS;

		GaussianBlurFilter() {}
		~GaussianBlurFilter() {}

		virtual void init();
		virtual void update(const float deltaTime);

	private:
		std::shared_ptr<Components::Camera> m_PostProcessCamera;
		std::vector<float> m_Weights;

		float gauss(float const& x, float const& sigma2);
		void updateShader();
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::GaussianBlurFilter, 0);
