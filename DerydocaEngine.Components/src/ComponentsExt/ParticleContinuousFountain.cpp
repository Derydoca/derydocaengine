#include "EngineComponentsPch.h"
#include "ParticleContinuousFountain.h"

#include <GL\glew.h>
#include <iostream>
#include "Rendering\Shader.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Ext
{

	ParticleContinuousFountain::ParticleContinuousFountain()
	{
	}

	ParticleContinuousFountain::~ParticleContinuousFountain()
	{
	}

	void ParticleContinuousFountain::update(const float deltaTime)
	{
		m_Time += deltaTime;
		m_LastDeltaTime = deltaTime;
	}

	void ParticleContinuousFountain::init()
	{
		m_Trans = getGameObject()->getTransform();

		initBuffers();
		m_RenderSub = m_Material->getShader()->getSubroutineIndex(GL_VERTEX_SHADER, "render");
		m_UpdateSub = m_Material->getShader()->getSubroutineIndex(GL_VERTEX_SHADER, "update");
	}

	void ParticleContinuousFountain::resetSimulation()
	{
		initBuffers();
		m_Time = 0.0f;
		m_LastDeltaTime = 0.0f;
		m_Material->setFloat("ParticleLifetime", m_Lifetime);
		m_Material->setVec3("Accel", m_Acceleration);
		m_Material->setFloat("MinParticleSize", m_ParticleSizeMin);
		m_Material->setFloat("MaxParticleSize", m_ParticleSizeMax);
	}

	void ParticleContinuousFountain::preRender()
	{
		m_Material->setVec3("Position", getGameObject()->getTransform()->getWorldPosition());

		// Update the shader's time variable
		m_Material->setFloat("Time", m_Time);

		m_Material->setVec3("EmitterPosition", m_Trans->getWorldPosition());
	}

	SERIALIZE_FUNC_LOAD(archive, ParticleContinuousFountain)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VelocityMin),
			SERIALIZE(m_VelocityMax),
			SERIALIZE(m_Angle),
			SERIALIZE(m_Lifetime),
			SERIALIZE(m_EmitterType),
			SERIALIZE(m_EmitterSize),
			SERIALIZE(m_Acceleration),
			SERIALIZE(m_ParticleSizeMin),
			SERIALIZE(m_ParticleSizeMax),
			SERIALIZE(m_SpawnRatePerSecond),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Texture)
		);

		const char* outputNames[] = { "Position", "Velocity", "StartTime" };
		auto shader = std::make_shared<Rendering::Shader>(m_Shader->getRawShaderName(), 3, outputNames);
		assert(shader);
		m_Material = std::make_shared<Rendering::Material>();
		m_Material->setShader(shader);
		m_Material->setFloat("ParticleLifetime", m_Lifetime);
		m_Material->setVec3("Accel", m_Acceleration);
		m_Material->setFloat("MinParticleSize", m_ParticleSizeMin);
		m_Material->setFloat("MaxParticleSize", m_ParticleSizeMax);

		m_Material->setTexture("ParticleTex", m_Texture.As<Rendering::Texture>());
	}

	SERIALIZE_FUNC_SAVE(archive, ParticleContinuousFountain)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_NumParticles),
			SERIALIZE(m_VelocityMin),
			SERIALIZE(m_VelocityMax),
			SERIALIZE(m_Angle),
			SERIALIZE(m_Lifetime),
			SERIALIZE(m_EmitterType),
			SERIALIZE(m_EmitterSize),
			SERIALIZE(m_Acceleration),
			SERIALIZE(m_ParticleSizeMin),
			SERIALIZE(m_ParticleSizeMax),
			SERIALIZE(m_SpawnRatePerSecond),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Texture)
		);
	}

	void ParticleContinuousFountain::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		m_Material->bind();
		m_Material->getShader()->updateViaActiveCamera(matrixStack);

		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		updateParticlePositions(m_LastDeltaTime);
		renderParticles();
	}

	void ParticleContinuousFountain::initBuffers()
	{
		// Generate the buffers
		glGenBuffers(2, m_PosBuf);
		glGenBuffers(2, m_VelBuf);
		glGenBuffers(2, m_StartTime);
		glGenBuffers(1, &m_InitVel);
		glGenBuffers(1, &m_InitPos);

		// Allocate space for the buffers
		int size = m_NumParticles * 3 * sizeof(GLfloat);
		glBindBuffer(GL_ARRAY_BUFFER, m_PosBuf[0]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_PosBuf[1]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_VelBuf[0]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_VelBuf[1]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_InitPos);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime[0]);
		glBufferData(GL_ARRAY_BUFFER, m_NumParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime[1]);
		glBufferData(GL_ARRAY_BUFFER, m_NumParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);

		// Fill the position data
		GLfloat* data = new GLfloat[m_NumParticles * 3];
		glm::vec3 worldPos = m_Trans->getWorldPosition();
		for (int i = 0; i < m_NumParticles; i++)
		{
			data[i * 3 + 0] = worldPos.x;
			data[i * 3 + 1] = worldPos.y;
			data[i * 3 + 2] = worldPos.z;

			if (m_EmitterType == ParticleEmitterType::Cube)
			{
				data[i * 3 + 0] += glm::mix(m_EmitterSize.x / 2, -m_EmitterSize.x / 2, randFloat());
				data[i * 3 + 1] += glm::mix(m_EmitterSize.y / 2, -m_EmitterSize.y / 2, randFloat());
				data[i * 3 + 2] += glm::mix(m_EmitterSize.z / 2, -m_EmitterSize.z / 2, randFloat());
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_PosBuf[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, m_InitPos);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		// Fill the first velocity buffer with random velocities
		for (int i = 0; i < m_NumParticles; i++)
		{

			glm::vec3 v = glm::vec3();
			switch (m_EmitterType)
			{
			case ParticleEmitterType::Cone:
				v = getVelocityFromCone();
				break;
			default:
				v = getVelocityFromCube();
				break;
			}

			data[3 * i] = v.x;
			data[3 * i + 1] = v.y;
			data[3 * i + 2] = v.z;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VelBuf[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		delete[] data;
		data = new GLfloat[m_NumParticles];
		float time = 0.0f;
		float rate = 1 / m_SpawnRatePerSecond;

		for (int i = 0; i < m_NumParticles; i++)
		{
			data[i] = time;
			time += rate;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_NumParticles * sizeof(float), data);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete[] data;

		// Create vertex arrays for each set of buffers
		glGenVertexArrays(2, m_ParticleArray);

		// Set up particle array 0
		glBindVertexArray(m_ParticleArray[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_PosBuf[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_VelBuf[0]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime[0]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_InitPos);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);

		// Set up particle array 0
		glBindVertexArray(m_ParticleArray[1]);
		glBindBuffer(GL_ARRAY_BUFFER, m_PosBuf[1]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_VelBuf[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_StartTime[1]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m_InitVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_InitPos);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);

		// Setup the feedback objects
		glGenTransformFeedbacks(2, m_Feedback);

		// Transform feedback 0
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_PosBuf[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_VelBuf[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_StartTime[0]);

		// Transform feedback 1
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_PosBuf[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_VelBuf[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_StartTime[1]);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

		GLint value;
		glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &value);
		LOG_INFO("MAX_TRANSFORM_FEEDBACK_BUFFERS = {}", value);
	}

	float ParticleContinuousFountain::randFloat()
	{
		return (float)rand() / RAND_MAX;
	}

	void ParticleContinuousFountain::updateParticlePositions(float const& deltaTime)
	{
		m_Material->setSubroutine(GL_VERTEX_SHADER, m_UpdateSub);

		m_Material->setFloat("Time", m_Time);
		m_Material->setFloat("H", deltaTime);

		m_Material->bind();

		glEnable(GL_RASTERIZER_DISCARD);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[m_DrawBuf]);

		glBeginTransformFeedback(GL_POINTS);
		glBindVertexArray(m_ParticleArray[1 - m_DrawBuf]);
		glDrawArrays(GL_POINTS, 0, m_NumParticles);
		glEndTransformFeedback();

		glDisable(GL_RASTERIZER_DISCARD);
	}

	void ParticleContinuousFountain::renderParticles()
	{
		glDisable(GL_DEPTH_TEST);
		m_Material->setSubroutine(GL_VERTEX_SHADER, m_RenderSub);

		// Setup the other stuff
		m_Material->bind();

		glBindVertexArray(m_ParticleArray[m_DrawBuf]);
		glDrawTransformFeedback(GL_POINTS, m_Feedback[m_DrawBuf]);

		// Swap buffers
		m_DrawBuf = 1 - m_DrawBuf;

		glEnable(GL_DEPTH_TEST);
	}

	glm::vec3 ParticleContinuousFountain::getVelocityFromCone()
	{
		glm::vec3 v(0.0f);
		glm::vec3 velocity;
		float theta;
		float phi;

		// Pick the direction of the particles
		theta = glm::mix(0.0f, glm::pi<float>() / m_Angle, randFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		// Scale to set the magnitude of the velocity
		velocity = mix(m_VelocityMin, m_VelocityMax, randFloat());
		v = normalize(v) * velocity;

		return v;
	}

	glm::vec3 ParticleContinuousFountain::getVelocityFromCube()
	{
		return mix(m_VelocityMin, m_VelocityMax, randFloat());
	}

}
