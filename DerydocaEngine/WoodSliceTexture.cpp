#include "WoodSliceTexture.h"
#include "YamlTools.h"

WoodSliceTexture::WoodSliceTexture()
{
}

WoodSliceTexture::~WoodSliceTexture()
{
}

void WoodSliceTexture::init()
{
	m_meshRenderer = getComponent<MeshRenderer>();
}

void WoodSliceTexture::deserialize(YAML::Node const& compNode)
{
	YAML::Node slicePositionNode = compNode["slicePosition"];
	if (slicePositionNode)
	{
		m_sliceMatrixTransform.setPos(slicePositionNode.as<vec3>());
	}

	YAML::Node sliceRotationNode = compNode["sliceRotation"];
	if (sliceRotationNode)
	{
		m_sliceMatrixTransform.setQuat(sliceRotationNode.as<fquat>());
	}

	YAML::Node sliceScaleNode = compNode["sliceScale"];
	if (sliceScaleNode)
	{
		m_sliceMatrixTransform.setScale(sliceScaleNode.as<vec3>());
	}
}

void WoodSliceTexture::preRender()
{
	m_meshRenderer->getMaterial()->setMat4("Slice", m_sliceMatrixTransform.getModel());
}
