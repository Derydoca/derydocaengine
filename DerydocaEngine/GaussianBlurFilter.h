#pragma once
#include "GameComponent.h"
#include "Camera.h"

class GaussianBlurFilter : public GameComponent
{
public:
	GENINSTANCE(GaussianBlurFilter);

	GaussianBlurFilter();
	~GaussianBlurFilter();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);
	virtual void update(float deltaTime);
private:
	Camera* m_postProcessCamera;
	float m_weights [5];

	float gauss(float x, float sigma2);
	void updateShader();
};

