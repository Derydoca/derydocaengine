#pragma once
#include "GameComponent.h"

class ParticleContinuousFountain : public GameComponent
{
public:
	GENINSTANCE(ParticleContinuousFountain);

	ParticleContinuousFountain();
	~ParticleContinuousFountain();

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
	float m_lastDeltaTime = 0.0f;
	vec3 m_acceleration = vec3(0.0f, -0.4f, 0.0f);
	GLuint m_posBuf[2];
	GLuint m_velBuf[2];
	GLuint m_startTime[2];
	GLuint m_initVel;
	GLuint m_particleArray[2];
	GLuint m_feedback[2];
	GLuint m_drawBuf = 1;
	GLuint m_updateSub;
	GLuint m_renderSub;
	Transform* m_trans;

	void initBuffers();
	float randFloat();
	void updateParticlePositions(float deltaTime);
	void renderParticles();
};
