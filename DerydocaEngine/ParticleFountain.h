#pragma once
#include "GameComponent.h"
#include "Keyboard.h"

class ParticleFountain : public GameComponent
{
public:
	GENINSTANCE(ParticleFountain);

	ParticleFountain();
	~ParticleFountain();

	virtual void init();
	virtual void update(float deltaTime);
	virtual void preRender();
	virtual void deserialize(YAML::Node compNode);
	virtual void render(MatrixStack* matrixStack);
private:
	int m_numParticles = 1000;
	Material* m_material;
	float m_time = 0.0f;
	float m_lifetime = 5.0f;
	float m_velocityMin = 1.25f;
	float m_velocityMax = 1.5f;
	float m_angle = 6.0f;
	GLuint m_vao;
	GLuint m_initVel;
	GLuint m_startTime;
	Keyboard* m_keyboard;

	void initBuffers();
	float randFloat();
};
