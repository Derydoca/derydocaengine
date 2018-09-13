#pragma once

#include "GameComponent.h"
#include "RenderTexture.h"
#include "MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	class EdgeDetectionFilter : public GameComponent
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
		DerydocaEngine::Components::Camera* m_postProcessCamera;
	};

}
