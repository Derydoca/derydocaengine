#pragma once
#include "GameComponent.h"
#include "RenderTexture.h"

class GammaCorrectionFilter : public GameComponent
{
public:
	GENINSTANCE(GammaCorrectionFilter);

	GammaCorrectionFilter();
	~GammaCorrectionFilter();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);
	virtual void update(float deltaTime);
private:
	float m_gamma = 2.0;
	Camera* m_postProcessCamera;

	void updateShader();
};

