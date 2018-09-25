#pragma once

#include "GameComponent.h"
#include "RenderTexture.h"
#include "MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	class EdgeDetectionFilter : public Components::GameComponent, Components::SelfRegister<EdgeDetectionFilter>
	{
	public:
		GENINSTANCE(EdgeDetectionFilter);

		EdgeDetectionFilter() {}
		~EdgeDetectionFilter() {}

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void update(float const& deltaTime);
	private:
		void updateShader();

		float m_edgeThreshold;
		Components::Camera* m_postProcessCamera;
	};

}
