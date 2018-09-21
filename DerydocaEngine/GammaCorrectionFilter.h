#pragma once
#include "GameComponent.h"
#include "RenderTexture.h"
#include "Camera.h"

namespace DerydocaEngine::Ext
{

	class GammaCorrectionFilter : public Components::GameComponent
	{
	public:
		GENINSTANCE(GammaCorrectionFilter);

		GammaCorrectionFilter() {}
		~GammaCorrectionFilter() {}

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void update(float const& deltaTime);
	private:
		float m_gamma = 2.0;
		Components::Camera* m_postProcessCamera;

		void updateShader();
	};

}
