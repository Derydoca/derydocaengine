#include "EngineComponentsPch.h"
#include "ParticleFountain.h"

#include <GL\glew.h>
#include "Input\InputManager.h"
#include "Rendering\Shader.h"
#include "sdl2\SDL.h"

namespace DerydocaEngine::Ext
{

	ParticleFountain::ParticleFountain()
	{
	}

	ParticleFountain::~ParticleFountain()
	{
	}

	void ParticleFountain::update(const float deltaTime)
	{
		m_time += deltaTime;
	}

	void ParticleFountain::resetSimulation()
	{
		m_time = 0.0f;
		initBuffers();
		m_material->setFloat("ParticleLifetime", m_lifetime);
	}

	void ParticleFountain::init()
	{
		initBuffers();

		m_keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	void ParticleFountain::preRender()
	{
		m_material->setVec3("Position", getGameObject()->getTransform()->getWorldPos());

		// Update the shader's time variable
		m_material->setFloat("Time", m_time);
	}

	void ParticleFountain::deserialize(const YAML::Node& compNode)
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

		auto shader = getResourcePointer<Rendering::Shader>(compNode, "shader");
		assert(shader);
		m_material = std::make_shared<Rendering::Material>();
		m_material->setShader(shader);
		m_material->setFloat("ParticleLifetime", m_lifetime);

		std::shared_ptr<Rendering::Texture> m_tex = getResourcePointer<Rendering::Texture>(compNode, "texture");
		m_material->setTexture("ParticleTex", m_tex);
	}

	void ParticleFountain::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		glDisable(GL_DEPTH_TEST);
		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(10.0f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_POINTS, 0, m_numParticles);
		glFinish();
		glEnable(GL_DEPTH_TEST);
	}

	void ParticleFountain::initBuffers()
	{
		// Generate the buffers
		glGenBuffers(1, &m_initVel);
		glGenBuffers(1, &m_startTime);

		int size = m_numParticles * 3 * sizeof(float);
		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_startTime);
		glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(float), NULL, GL_STATIC_DRAW);

		// Fill the first buffer with random velocities
		glm::vec3 v(0.0f);
		float velocity, theta, phi;

		GLfloat* data = new GLfloat[m_numParticles * 3];
		for (int i = 0; i < m_numParticles; i++)
		{
			// Pick the direction of the particles
			theta = glm::mix(0.0f, glm::pi<float>() / m_angle, randFloat());
			phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

			v.x = sinf(theta) * cosf(phi);
			v.y = cosf(theta);
			v.z = sinf(theta) * sinf(phi);

			// Scale to set the magnitude of the velocity
			velocity = glm::mix(m_velocityMin, m_velocityMax, randFloat());
			v = normalize(v) * velocity;

			data[3 * i] = v.x;
			data[3 * i + 1] = v.y;
			data[3 * i + 2] = v.z;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		delete[] data;

		data = new GLfloat[m_numParticles];
		float time = 0.0f;
		float rate = 0.00075f;

		for (int i = 0; i < m_numParticles; i++)
		{
			data[i] = time;
			time += rate;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(float), data);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete[] data;

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	float ParticleFountain::randFloat()
	{
		return (float)rand() / RAND_MAX;
	}

}
