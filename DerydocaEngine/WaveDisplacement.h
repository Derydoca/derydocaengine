#pragma once
#include "GameComponent.h"
class WaveDisplacement : public GameComponent
{
public:
	GENINSTANCE(WaveDisplacement);

	WaveDisplacement();
	~WaveDisplacement();

	virtual void init();
	virtual void update(float deltaTime);
	virtual void preRender();
	virtual void deserialize(YAML::Node compNode);
private:
	float m_k = 1.0f;
	float m_velocity = 1.0f;
	float m_amplitude = 0.5f;
	float m_time = 0.0f;
	Material* m_material;
};

