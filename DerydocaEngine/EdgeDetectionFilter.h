#pragma once

#include "GameComponent.h"
#include "RenderTexture.h"
#include "MeshRenderer.h"

using namespace std;

class EdgeDetectionFilter : public GameComponent
{
public:
	GENINSTANCE(EdgeDetectionFilter);

	EdgeDetectionFilter();
	~EdgeDetectionFilter();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);
	virtual void update(float deltaTime);
private:
	void updateShader();

	float m_edgeThreshold;
	Camera* m_postProcessCamera;
};

