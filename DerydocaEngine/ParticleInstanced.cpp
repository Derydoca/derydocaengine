#include "ParticleInstanced.h"

#include <GL\glew.h>
#include "InputManager.h"
#include "LightManager.h"
#include "Shader.h"

namespace DerydocaEngine::Ext
{

	ParticleInstanced::ParticleInstanced()
	{
	}

	ParticleInstanced::~ParticleInstanced()
	{
		delete m_material;
	}

	void ParticleInstanced::update(float const& deltaTime)
	{
		m_time += deltaTime;

		if (m_keyboard->isKeyDownFrame(SDLK_r))
		{
			m_time = 0.0f;
		}
	}

	void ParticleInstanced::init()
	{
		initBuffers();

		m_keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	void ParticleInstanced::preRender()
	{
		// Update the shader's time variable
		m_material->setFloat("Time", m_time);

		m_material->setVec3("Gravity", m_gravity);
	}

	void ParticleInstanced::deserialize(YAML::Node const& compNode)
	{
		YAML::Node numParticlesNode = compNode["numParticles"];
		if (numParticlesNode)
		{
			m_numParticles = numParticlesNode.as<int>();
		}

		YAML::Node velocityMaxNode = compNode["velocityMax"];
		if (velocityMaxNode)
		{
			m_velocityMax = velocityMaxNode.as<float>();
		}

		YAML::Node velocityMinNode = compNode["velocityMin"];
		if (velocityMinNode)
		{
			m_velocityMin = velocityMinNode.as<float>();
		}

		YAML::Node angleNode = compNode["angle"];
		if (angleNode)
		{
			m_angle = angleNode.as<float>();
		}

		Rendering::Shader* shader = getResourceObject<Rendering::Shader>(compNode, "shader");
		assert(shader);
		m_material = new Rendering::Material();
		m_material->setShader(shader);
		m_material->setFloat("ParticleLifetime", m_lifetime);
		m_material->setVec4("Material.Kd", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_material->setVec4("Material.Ka", glm::vec4(0.05f, 0.05f, 0.0f, 1.0f));
		m_material->setVec4("Material.Ks", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
		m_material->setFloat("Material.Shininess", 50.0f);

		m_mesh = getResourceObject<Rendering::Mesh>(compNode, "mesh");
	}

	void ParticleInstanced::render(Rendering::MatrixStack * const& matrixStack)
	{
		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		Rendering::LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

		glBindVertexArray(m_mesh->getVao());
		glDrawElementsInstanced(GL_TRIANGLES, m_mesh->getNumIndices(), GL_UNSIGNED_INT, 0, m_numParticles);
	}

	void ParticleInstanced::initBuffers()
	{
		// Generate the buffers
		glGenBuffers(1, &m_initVel);
		glGenBuffers(1, &m_startTime);

		// Allocate space for all buffers
		int size = m_numParticles * 3 * sizeof(float);
		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_startTime);
		glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(float), NULL, GL_STATIC_DRAW);

		// Fill the first velocity buffer with random velocities
		glm::vec3 v(0.0f);
		float velocity;
		float theta;
		float phi;

		GLfloat* data = new GLfloat[m_numParticles * 3];
		for (int i = 0; i < m_numParticles; i++)
		{
			theta = glm::mix(0.0f, glm::pi<float>() / m_angle, randFloat());
			phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

			v.x = sinf(theta) * cosf(phi);
			v.y = cosf(theta);
			v.z = sinf(theta) * sinf(phi);

			velocity = glm::mix(m_velocityMin, m_velocityMax, randFloat());
			v = normalize(v) * velocity;

			data[3 * i + 0] = v.x;
			data[3 * i + 1] = v.y;
			data[3 * i + 2] = v.z;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		// Fill the start time buffer
		delete[] data;
		data = new GLfloat[m_numParticles];
		float time = 0.0f;
		float rate = 0.01f;
		for (int i = 0; i < m_numParticles; i++)
		{
			data[i] = time;
			time += rate;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_startTime);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(float), data);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete[] data;

		// Attach these to the torus's vertex array
		glBindVertexArray(m_mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);

		glBindVertexArray(0);
	}

	float ParticleInstanced::randFloat()
	{
		return (float)rand() / RAND_MAX;
	}

}
