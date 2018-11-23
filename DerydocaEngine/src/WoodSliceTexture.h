#pragma once
#include "Components\GameComponent.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	class WoodSliceTexture : public Components::GameComponent, Components::SelfRegister<WoodSliceTexture>
	{
	public:
		GENINSTANCE(WoodSliceTexture);
		WoodSliceTexture() {}
		~WoodSliceTexture() {}

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void preRender();
	private:
		Components::Transform m_sliceMatrixTransform;
		Components::MeshRenderer* m_meshRenderer;
	};

}
