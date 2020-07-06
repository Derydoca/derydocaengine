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

	void WoodSliceTexture::deserialize(const YAML::Node& compNode)
	{
		YAML::Node slicePositionNode = compNode["slicePosition"];
		if (slicePositionNode)
		{
			m_SliceMatrixTransform->setPosition(slicePositionNode.as<glm::vec3>());
		}

		YAML::Node sliceRotationNode = compNode["sliceRotation"];
		if (sliceRotationNode)
		{
			m_SliceMatrixTransform->setQuaternion(sliceRotationNode.as<glm::fquat>());
		}

		YAML::Node sliceScaleNode = compNode["sliceScale"];
		if (sliceScaleNode)
		{
			m_SliceMatrixTransform->setScale(sliceScaleNode.as<glm::vec3>());
		}
	}

	void WoodSliceTexture::preRender()
	{
		m_MeshRenderer->getMaterial()->setMat4("Slice", m_SliceMatrixTransform->getModel());
	}

}
