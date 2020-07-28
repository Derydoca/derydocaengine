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
		m_Time += deltaTime;
	}

	void ParticleFountain::resetSimulation()
	{
		m_Time = 0.0f;
		initBuffers();
		m_Material->setFloat("ParticleLifetime", m_Lifetime);
	}

	void ParticleFountain::init()
	{
		initBuffers();

		m_Keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	void ParticleFountain::preRender()
	{
		m_Material->setVec3("Position", getGameObject()->getTransform()->getWorldPosition());

		// Update the shader's time variable
		m_Material->setFloat("Time", m_Time);
	}

	SERIALIZE_FUNC_LOAD(archive, ParticleFountain)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VelocityMin),
			SERIALIZE(m_VelocityMax),
			SERIALIZE(m_Angle),
			SERIALIZE(m_Lifetime),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Texture)
		);

		m_Material = std::make_shared<Rendering::Material>();
		// This may break having multiple instances of this component, but this component is only for illustrative purposes only
		{
			//auto shader = m_Shader.As<Rendering::Shader>();
			//m_Material->setShader(shader);
		}
		m_Material->setShader(m_Shader);
		m_Material->setFloat("ParticleLifetime", m_Lifetime);

		auto texture = m_Texture.As<Rendering::Texture>();
		m_Material->setTexture("ParticleTex", texture);
	}

	SERIALIZE_FUNC_SAVE(archive, ParticleFountain)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VelocityMin),
			SERIALIZE(m_VelocityMax),
			SERIALIZE(m_Angle),
			SERIALIZE(m_Lifetime),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Texture)
		);
	}

	void ParticleFountain::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		glDisable(GL_DEPTH_TEST);
		m_Material->bind();
		m_Material->getShader()->updateViaActiveCamera(matrixStack);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(10.0f);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_POINTS, 0, m_NumParticles);
		glFinish();
		glEnable(GL_DEPTH_TEST);
	}

	void ParticleFountain::initBuffers()
	{
		// Generate the buffers
		glGenBuffers(1, &m_InitVel);
		glGenBuffers(1, &m_StartTime);

		int size = m_NumParticles * 3 * sizeof(float);
		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime);
		glBufferData(GL_ARRAY_BUFFER, m_NumParticles * sizeof(float), NULL, GL_STATIC_DRAW);

		// Fill the first buffer with random velocities
		glm::vec3 v(0.0f);
		float velocity, theta, phi;

		GLfloat* data = new GLfloat[m_NumParticles * 3];
		for (int i = 0; i < m_NumParticles; i++)
		{
			// Pick the direction of the particles
			theta = glm::mix(0.0f, glm::pi<float>() / m_Angle, randFloat());
			phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

			v.x = sinf(theta) * cosf(phi);
			v.y = cosf(theta);
			v.z = sinf(theta) * sinf(phi);

			// Scale to set the magnitude of the velocity
			velocity = glm::mix(m_VelocityMin, m_VelocityMax, randFloat());
			v = normalize(v) * velocity;

			data[3 * i] = v.x;
			data[3 * i + 1] = v.y;
			data[3 * i + 2] = v.z;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		delete[] data;

		data = new GLfloat[m_NumParticles];
		float time = 0.0f;
		float rate = 0.00075f;

		for (int i = 0; i < m_NumParticles; i++)
		{
			data[i] = time;
			time += rate;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_NumParticles * sizeof(float), data);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete[] data;

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	float ParticleFountain::randFloat()
	{
		return (float)rand() / RAND_MAX;
	}

}
