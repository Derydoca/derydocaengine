#pragma once
#include <glm\glm.hpp>
#include "GameComponent.h"
#include "Texture.h"
//#include "Shader.h"
#include "Material.h"

using namespace std;
using namespace glm;

class ParticleSystem : public GameComponent
{
public:
	GENINSTANCE(ParticleSystem);

	ParticleSystem();
	~ParticleSystem();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);
	void render(MatrixStack* matrixStack);
private:
	unsigned int m_numParticles;
	float m_size2;
	GLuint m_vao;
	GLuint m_vertexArrayBuffers[1];
	vec3 m_volumeSize;
	vec3* m_particleLocations;
	Texture* m_texture;
	//Shader* m_shader;
	Material* m_material;
};