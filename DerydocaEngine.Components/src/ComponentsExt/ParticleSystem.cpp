#include "EngineComponentsPch.h"
#include "ParticleSystem.h"

#include <GL\glew.h>
#include "Rendering\LightManager.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	static float RandomFloat(float const& min, float const& max);

	static float RandomFloat(float const& min, float const& max)
	{
		float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		return (r * (-min + max)) + min;
	}

	ParticleSystem::ParticleSystem()
	{
	}

	ParticleSystem::~ParticleSystem()
	{
		delete m_ParticleLocations;
		glDeleteVertexArrays(1, &m_VAO);
	}

	void ParticleSystem::init()
	{
		// Create the array of particles
		m_ParticleLocations = new glm::vec3[m_NumParticles];

		// Set positions to randomly generated positions within the volume size;
		for (int i = 0; i < m_NumParticles; i++)
		{
			float posx = RandomFloat(-m_VolumeSize.x / 2, m_VolumeSize.x / 2);
			float posy = RandomFloat(-m_VolumeSize.y / 2, m_VolumeSize.y / 2);
			float posz = RandomFloat(-m_VolumeSize.z / 2, m_VolumeSize.z / 2);
			m_ParticleLocations[i] = glm::vec3(posx, posy, posz);
		}

		glGenBuffers(1, m_VertexArrayBuffers);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, m_NumParticles * sizeof(glm::vec3), m_ParticleLocations, GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	SERIALIZE_FUNC_LOAD(archive, ParticleSystem)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VolumeSize),
			SERIALIZE(m_Size2),
			SERIALIZE(m_Texture),
			SERIALIZE(m_Shader)
		);

		m_Material = std::make_shared<Rendering::Material>();
		m_Material->setShader(m_Shader);
		m_Material->setFloat("Size2", m_Size2);
		m_Material->setTexture("SpriteTex", GetTexture());
	}

	SERIALIZE_FUNC_SAVE(archive, ParticleSystem)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VolumeSize),
			SERIALIZE(m_Size2),
			SERIALIZE(m_Texture),
			SERIALIZE(m_Shader)
		);
	}

	void ParticleSystem::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		glDisable(GL_DEPTH_TEST);

		m_Material->bind();
		m_Material->getShader()->updateViaActiveCamera(matrixStack);
		Rendering::LightManager::getInstance().bindLightsToShader(m_Material->getShader());

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_POINTS, 0, m_NumParticles);
		glEnable(GL_DEPTH_TEST);
		glFinish();
	}

	void ParticleSystem::reset()
	{
		m_Material->setFloat("Size2", m_Size2);
		init();
	}

}
