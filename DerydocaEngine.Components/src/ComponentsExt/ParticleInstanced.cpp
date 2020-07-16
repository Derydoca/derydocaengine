#include "EngineComponentsPch.h"
#include "ParticleInstanced.h"

#include <GL\glew.h>
#include "Input\InputManager.h"
#include "Rendering\LightManager.h"
#include "Rendering\Material.h"
#include "Rendering\Shader.h"
#include "sdl2\SDL.h"

namespace DerydocaEngine::Ext
{

	ParticleInstanced::ParticleInstanced()
	{
	}

	ParticleInstanced::~ParticleInstanced()
	{
	}

	void ParticleInstanced::update(const float deltaTime)
	{
		m_Time += deltaTime;
	}

	void ParticleInstanced::resetSimulation()
	{
		m_Time = 0.0f;
		initBuffers();
		m_Material->setFloat("ParticleLifetime", m_Lifetime);
	}

	void ParticleInstanced::init()
	{
		initBuffers();

		m_Keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	void ParticleInstanced::preRender()
	{
		// Update the shader's time variable
		m_Material->setFloat("Time", m_Time);

		m_Material->setVec3("Gravity", m_Gravity);
	}

	SERIALIZE_FUNC_LOAD(archive, ParticleInstanced)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VelocityMin),
			SERIALIZE(m_VelocityMax),
			SERIALIZE(m_Angle),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Mesh)
		);

		auto shader = m_Shader.As<Rendering::Shader>();
		assert(shader);
		m_Material = std::make_shared<Rendering::Material>();
		m_Material->setShader(shader);
		m_Material->setFloat("ParticleLifetime", m_Lifetime);
		m_Material->setVec4("Material.Kd", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_Material->setVec4("Material.Ka", glm::vec4(0.05f, 0.05f, 0.0f, 1.0f));
		m_Material->setVec4("Material.Ks", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
		m_Material->setFloat("Material.Shininess", 50.0f);
	}

	SERIALIZE_FUNC_SAVE(archive, ParticleInstanced)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VelocityMin),
			SERIALIZE(m_VelocityMax),
			SERIALIZE(m_Angle),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Mesh)
		);
	}

	void ParticleInstanced::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		auto mesh = m_Mesh.As<Rendering::Mesh>();

		m_Material->bind();
		m_Material->getShader()->updateViaActiveCamera(matrixStack);
		Rendering::LightManager::getInstance().bindLightsToShader(m_Material->getShader());

		glBindVertexArray(mesh->getVao());
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(mesh->getNumIndices()), GL_UNSIGNED_INT, 0, m_NumParticles);
	}

	void ParticleInstanced::initBuffers()
	{
		auto mesh = m_Mesh.As<Rendering::Mesh>();

		// Generate the buffers
		glGenBuffers(1, &m_InitVel);
		glGenBuffers(1, &m_StartTime);

		// Allocate space for all buffers
		int size = m_NumParticles * 3 * sizeof(float);
		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime);
		glBufferData(GL_ARRAY_BUFFER, m_NumParticles * sizeof(float), NULL, GL_STATIC_DRAW);

		// Fill the first velocity buffer with random velocities
		glm::vec3 v(0.0f);
		float velocity;
		float theta;
		float phi;

		GLfloat* data = new GLfloat[m_NumParticles * 3];
		for (int i = 0; i < m_NumParticles; i++)
		{
			theta = glm::mix(0.0f, glm::pi<float>() / m_Angle, randFloat());
			phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

			v.x = sinf(theta) * cosf(phi);
			v.y = cosf(theta);
			v.z = sinf(theta) * sinf(phi);

			velocity = glm::mix(m_VelocityMin, m_VelocityMax, randFloat());
			v = normalize(v) * velocity;

			data[3 * i + 0] = v.x;
			data[3 * i + 1] = v.y;
			data[3 * i + 2] = v.z;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		// Fill the start time buffer
		delete[] data;
		data = new GLfloat[m_NumParticles];
		float time = 0.0f;
		float rate = 0.01f;
		for (int i = 0; i < m_NumParticles; i++)
		{
			data[i] = time;
			time += rate;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_NumParticles * sizeof(float), data);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete[] data;

		// Attach these to the torus's vertex array
		glBindVertexArray(mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime);
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
