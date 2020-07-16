#pragma once

#include "Components\GameComponent.h"
#include "Rendering\RenderTexture.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	class EdgeDetectionFilter : public Components::GameComponent, Components::SelfRegister<EdgeDetectionFilter>
	{
	public:
		GENINSTANCE(EdgeDetectionFilter);
		SERIALIZE_FUNC_DEFINITIONS;

		EdgeDetectionFilter();
		~EdgeDetectionFilter();

		virtual void init();
		virtual void update(const float deltaTime);

		float& getEdgeThreshold() { return m_EdgeThreshold; }

	private:
		void updateShader();

		float m_EdgeThreshold;
		std::shared_ptr<Components::Camera> m_PostProcessCamera;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::EdgeDetectionFilter, 0);
