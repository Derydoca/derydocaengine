#pragma once

#include "GameComponent.h"
#include "RenderTexture.h"
#include "MeshRenderer.h"

class EdgeDetectionFilter : public GameComponent
{
public:
	GENINSTANCE(EdgeDetectionFilter);

	EdgeDetectionFilter();
	~EdgeDetectionFilter();

	virtual void init();
	virtual void deserialize(YAML::Node const& compNode);
	virtual void update(float const& deltaTime);
private:
	void updateShader();

	float m_edgeThreshold;
	Camera* m_postProcessCamera;
};

