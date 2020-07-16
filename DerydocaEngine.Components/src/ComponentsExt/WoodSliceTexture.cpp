#include "EngineComponentsPch.h"
#include "WoodSliceTexture.h"

#include "Rendering\Material.h"
#include "Helpers\YamlTools.h"

namespace DerydocaEngine::Ext
{

	WoodSliceTexture::WoodSliceTexture() :
		m_SliceMatrixTransform(std::make_shared<Components::Transform>()),
		m_MeshRenderer()
	{
	}

	WoodSliceTexture::~WoodSliceTexture()
	{
	}

	void WoodSliceTexture::init()
	{
		m_MeshRenderer = getComponentInChildren<Components::MeshRenderer>();
	}

	void WoodSliceTexture::preRender()
	{
		m_MeshRenderer->getMaterial()->setMat4("Slice", m_SliceMatrixTransform->getModel());
	}

	SERIALIZE_FUNC_LOAD(archive, WoodSliceTexture)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_SliceMatrixTransform)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, WoodSliceTexture)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_SliceMatrixTransform)
		);
	}

}
