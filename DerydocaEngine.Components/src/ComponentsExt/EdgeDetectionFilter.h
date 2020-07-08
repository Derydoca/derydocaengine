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

		EdgeDetectionFilter();
		~EdgeDetectionFilter();

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void update(const float deltaTime);

		float& getEdgeThreshold() { return m_EdgeThreshold; }

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_EdgeThreshold)
		);
	private:
		void updateShader();

		float m_EdgeThreshold;
		std::shared_ptr<Components::Camera> m_PostProcessCamera;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::EdgeDetectionFilter, 0);
