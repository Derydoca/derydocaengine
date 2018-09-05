#pragma once
#include "GameComponent.h"
#include "MeshRenderer.h"

class WoodSliceTexture : public GameComponent
{
public:
	GENINSTANCE(WoodSliceTexture);
	WoodSliceTexture();
	~WoodSliceTexture();

	virtual void init();
	virtual void deserialize(YAML::Node const& compNode);
	virtual void preRender();
private:
	Transform m_sliceMatrixTransform;
	MeshRenderer* m_meshRenderer;
};
