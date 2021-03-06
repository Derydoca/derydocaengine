#pragma once
#include "Components\GameComponent.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	class WoodSliceTexture : public Components::GameComponent, Components::SelfRegister<WoodSliceTexture>
	{
	public:
		GENINSTANCE(WoodSliceTexture);
		SERIALIZE_FUNC_DEFINITIONS;
		WoodSliceTexture();
		~WoodSliceTexture();

		virtual void init();
		virtual void preRender();

		std::shared_ptr<Components::Transform> getSliceMatrixTransform() { return m_SliceMatrixTransform; }
	
	private:
		std::shared_ptr<Components::Transform> m_SliceMatrixTransform;
		std::shared_ptr<Components::MeshRenderer> m_MeshRenderer;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::WoodSliceTexture, 0);
