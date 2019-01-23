#include "EngineComponentsPch.h"
#include "WoodSliceTexture.h"

#include "Rendering\Material.h"
#include "Helpers\YamlTools.h"

namespace DerydocaEngine::Ext
{

	WoodSliceTexture::WoodSliceTexture() :
		m_sliceMatrixTransform(std::make_shared<Components::Transform>()),
		m_meshRenderer()
	{
	}

	WoodSliceTexture::~WoodSliceTexture()
	{
	}

	void WoodSliceTexture::init()
	{
		m_meshRenderer = getComponent<Components::MeshRenderer>();
	}

	void WoodSliceTexture::deserialize(const YAML::Node& compNode)
	{
		YAML::Node slicePositionNode = compNode["slicePosition"];
		if (slicePositionNode)
		{
			m_sliceMatrixTransform->setPos(slicePositionNode.as<glm::vec3>());
		}

		YAML::Node sliceRotationNode = compNode["sliceRotation"];
		if (sliceRotationNode)
		{
			m_sliceMatrixTransform->setQuat(sliceRotationNode.as<glm::fquat>());
		}

		YAML::Node sliceScaleNode = compNode["sliceScale"];
		if (sliceScaleNode)
		{
			m_sliceMatrixTransform->setScale(sliceScaleNode.as<glm::vec3>());
		}
	}

	void WoodSliceTexture::preRender()
	{
		m_meshRenderer->getMaterial()->setMat4("Slice", m_sliceMatrixTransform->getModel());
	}

}
