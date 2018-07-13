#pragma once
#include "GameComponent.h"

class ParticleContinuousFountain : public GameComponent
{
public:
	enum ParticleEmitterType
	{
		Point = 0,
		Cube = 1,
		Cone = 2
	};

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
	vec3 m_velocityMin = vec3(1.25f);
	vec3 m_velocityMax = vec3(1.5f);
	float m_angle = 6.0f;
	float m_lastDeltaTime = 0.0f;
	vec3 m_acceleration = vec3(0.0f, -0.4f, 0.0f);
	GLuint m_posBuf[2];
	GLuint m_velBuf[2];
	GLuint m_startTime[2];
	GLuint m_initVel;
	GLuint m_initPos;
	GLuint m_particleArray[2];
	GLuint m_feedback[2];
	GLuint m_drawBuf = 1;
	GLuint m_updateSub;
	GLuint m_renderSub;
	Transform* m_trans;
	ParticleEmitterType m_emitterType;
	vec3 m_emitterSize = vec3(1.0, 1.0, 1.0);
	float m_pointSize = 10.0f;

	void initBuffers();
	float randFloat();
	void updateParticlePositions(float deltaTime);
	void renderParticles();
	vec3 getVelocityFromCone();
	vec3 getVelocityFromCube();
};
