#pragma once
#include "GameComponent.h"
#include "Camera.h"

class NightVisionFilter : public GameComponent
{
public:
	GENINSTANCE(NightVisionFilter);

	NightVisionFilter();
	~NightVisionFilter();

	virtual void init();
	virtual void deserialize(YAML::Node const& compNode);
private:
	float m_radius = 100.0f;
	Camera* m_postProcessCamera;
};

