#include "ParticleSystem.h"
#include "LightManager.h"
#include "Shader.h"

static float RandomFloat(float min, float max);

static float RandomFloat(float min, float max)
{
	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return (r * (-min + max)) + min;
}

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
	delete(m_particleLocations);
	glDeleteVertexArrays(1, &m_vao);
}

void ParticleSystem::init()
{
	// Create the array of particles
	m_particleLocations = new vec3[m_numParticles];

	// Set positions to randomly generated positions within the volume size;
	for (uint i = 0; i < m_numParticles; i++)
	{
		float posx = RandomFloat(-m_volumeSize.x / 2, m_volumeSize.x / 2);
		float posy = RandomFloat(-m_volumeSize.y / 2, m_volumeSize.y / 2);
		float posz = RandomFloat(-m_volumeSize.z / 2, m_volumeSize.z / 2);
		m_particleLocations[i] = vec3(posx, posy, posz);
		//cout << i << ": " << m_particleLocations[i].x << ", " << m_particleLocations[i].y << ", " << m_particleLocations[i].z << endl;
	}

	glGenBuffers(1, m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(glm::vec3), m_particleLocations, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void ParticleSystem::deserialize(YAML::Node compNode)
{
	YAML::Node numParticlesNode = compNode["numParticles"];
	if (numParticlesNode)
	{
		m_numParticles = numParticlesNode.as<int>();
	}
	else
	{
		m_numParticles = 8;
	}

	YAML::Node volumeSizeNode = compNode["volumeSize"];
	if (volumeSizeNode)
	{
		m_volumeSize = volumeSizeNode.as<vec3>();
	}
	else
	{
		m_volumeSize = vec3(1.0);
	}

	YAML::Node size2Node = compNode["size2"];
	if (size2Node)
	{
		m_size2 = size2Node.as<float>();
	}
	else
	{
		m_size2 = 0.25f;
	}

	m_texture = loadResource<Texture*>(compNode, "texture");

	Shader* shader = loadResource<Shader*>(compNode, "shader");
	m_material = new Material();
	m_material->setShader(shader);
	m_material->setFloat("Size2", m_size2);
}

void ParticleSystem::render(MatrixStack* matrixStack)
{
	assert(getGameObject());

	m_material->bind();
	m_material->getShader()->updateViaActiveCamera(matrixStack);
	LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_numParticles);
	//glDrawElementsBaseVertex(GL_POINTS, m_numParticles, GL_UNSIGNED_INT, 0, 0);
	//glBindVertexArray(0);
	glFinish();
}
