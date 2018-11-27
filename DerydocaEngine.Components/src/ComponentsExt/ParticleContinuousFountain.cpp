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
		delete m_material;
	}

	void ParticleContinuousFountain::update(float const& deltaTime)
	{
		m_time += deltaTime;
		m_lastDeltaTime = deltaTime;
	}

	void ParticleContinuousFountain::init()
	{
		m_trans = getGameObject()->getTransform();

		initBuffers();
		m_renderSub = m_material->getShader()->getSubroutineIndex(GL_VERTEX_SHADER, "render");
		m_updateSub = m_material->getShader()->getSubroutineIndex(GL_VERTEX_SHADER, "update");
	}

	void ParticleContinuousFountain::preRender()
	{
		m_material->setVec3("Position", getGameObject()->getTransform()->getWorldPos());

		// Update the shader's time variable
		m_material->setFloat("Time", m_time);

		m_material->setVec3("EmitterPosition", m_trans->getWorldPos());
	}

	void ParticleContinuousFountain::deserialize(YAML::Node const& compNode)
	{
		YAML::Node numParticlesNode = compNode["numParticles"];
		if (numParticlesNode)
		{
			m_numParticles = numParticlesNode.as<int>();
		}

		YAML::Node velocityMaxNode = compNode["velocityMax"];
		if (velocityMaxNode)
		{
			if (velocityMaxNode.IsScalar())
			{
				float vel = velocityMaxNode.as<float>();
				m_velocityMax = glm::vec3(vel);
			}
			else
			{
				m_velocityMax = velocityMaxNode.as<glm::vec3>();
			}
		}

		YAML::Node velocityMinNode = compNode["velocityMin"];
		if (velocityMinNode)
		{
			if (velocityMinNode.IsScalar())
			{
				float vel = velocityMinNode.as<float>();
				m_velocityMin = glm::vec3(vel);
			}
			else
			{
				m_velocityMin = velocityMinNode.as<glm::vec3>();
			}
		}

		YAML::Node angleNode = compNode["angle"];
		if (angleNode)
		{
			m_angle = angleNode.as<float>();
		}

		YAML::Node lifetimeNode = compNode["lifetime"];
		if (lifetimeNode)
		{
			m_lifetime = lifetimeNode.as<float>();
		}

		YAML::Node emitterTypeNode = compNode["emitterType"];
		if (emitterTypeNode)
		{
			m_emitterType = (ParticleEmitterType)emitterTypeNode.as<int>();
		}

		YAML::Node emitterSizeNode = compNode["emitterSize"];
		if (emitterSizeNode)
		{
			m_emitterSize = emitterSizeNode.as<glm::vec3>();
		}

		YAML::Node accelerationNode = compNode["acceleration"];
		if (accelerationNode)
		{
			m_acceleration = accelerationNode.as<glm::vec3>();
		}

		YAML::Node particleSizeMinNode = compNode["particleSizeMin"];
		if (particleSizeMinNode)
		{
			m_particleSizeMin = particleSizeMinNode.as<float>();
		}

		YAML::Node particleSizeMaxNode = compNode["particleSizeMax"];
		if (particleSizeMaxNode)
		{
			m_particleSizeMax = particleSizeMaxNode.as<float>();
		}

		const char * outputNames[] = { "Position", "Velocity", "StartTime" };
		Resources::ShaderResource* shaderResource = getResource<Resources::ShaderResource*>(compNode, "shader");
		assert(shaderResource);
		Rendering::Shader* shader = new Rendering::Shader(shaderResource->getRawShaderName(), 3, outputNames);
		assert(shader);
		m_material = new Rendering::Material();
		m_material->setShader(shader);
		m_material->setFloat("ParticleLifetime", m_lifetime);
		m_material->setVec3("Accel", m_acceleration);
		m_material->setFloat("MinParticleSize", m_particleSizeMin);
		m_material->setFloat("MaxParticleSize", m_particleSizeMax);

		Rendering::Texture* m_tex = getResourceObject<Rendering::Texture>(compNode, "texture");
		m_material->setTexture("ParticleTex", m_tex);
	}

	void ParticleContinuousFountain::render(std::shared_ptr<Rendering::MatrixStack> const& matrixStack)
	{
		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);

		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		updateParticlePositions(m_lastDeltaTime);
		renderParticles();
	}

	void ParticleContinuousFountain::initBuffers()
	{
		// Generate the buffers
		glGenBuffers(2, m_posBuf);
		glGenBuffers(2, m_velBuf);
		glGenBuffers(2, m_startTime);
		glGenBuffers(1, &m_initVel);
		glGenBuffers(1, &m_initPos);

		// Allocate space for the buffers
		int size = m_numParticles * 3 * sizeof(GLfloat);
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[0]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[1]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[0]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[1]);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_initPos);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[0]);
		glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[1]);
		glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);

		// Fill the position data
		GLfloat* data = new GLfloat[m_numParticles * 3];
		glm::vec3 worldPos = m_trans->getWorldPos();
		for (int i = 0; i < m_numParticles; i++)
		{
			data[i * 3 + 0] = worldPos.x;
			data[i * 3 + 1] = worldPos.y;
			data[i * 3 + 2] = worldPos.z;

			if (m_emitterType == ParticleEmitterType::Cube)
			{
				data[i * 3 + 0] += glm::mix(m_emitterSize.x / 2, -m_emitterSize.x / 2, randFloat());
				data[i * 3 + 1] += glm::mix(m_emitterSize.y / 2, -m_emitterSize.y / 2, randFloat());
				data[i * 3 + 2] += glm::mix(m_emitterSize.z / 2, -m_emitterSize.z / 2, randFloat());
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, m_initPos);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		// Fill the first velocity buffer with random velocities
		for (int i = 0; i < m_numParticles; i++)
		{

			glm::vec3 v = glm::vec3();
			switch (m_emitterType)
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
		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

		delete[] data;
		data = new GLfloat[m_numParticles];
		float time = 0.0f;
		float rate = 0.01f;

		for (int i = 0; i < m_numParticles; i++)
		{
			data[i] = time;
			time += rate;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(float), data);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete[] data;

		// Create vertex arrays for each set of buffers
		glGenVertexArrays(2, m_particleArray);

		// Set up particle array 0
		glBindVertexArray(m_particleArray[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[0]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[0]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_initPos);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);

		// Set up particle array 0
		glBindVertexArray(m_particleArray[1]);
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[1]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[1]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m_initVel);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, m_initPos);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);

		// Setup the feedback objects
		glGenTransformFeedbacks(2, m_feedback);

		// Transform feedback 0
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[0]);

		// Transform feedback 1
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[1]);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

		GLint value;
		glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &value);
		std::cout << "MAX_TRANSFORM_FEEDBACK_BUFFERS = " << value << "\n";
	}

	float ParticleContinuousFountain::randFloat()
	{
		return (float)rand() / RAND_MAX;
	}

	void ParticleContinuousFountain::updateParticlePositions(float const& deltaTime)
	{
		m_material->setSubroutine(GL_VERTEX_SHADER, m_updateSub);

		m_material->setFloat("Time", m_time);
		m_material->setFloat("H", deltaTime);

		m_material->bind();

		glEnable(GL_RASTERIZER_DISCARD);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[m_drawBuf]);

		glBeginTransformFeedback(GL_POINTS);
		glBindVertexArray(m_particleArray[1 - m_drawBuf]);
		glDrawArrays(GL_POINTS, 0, m_numParticles);
		glEndTransformFeedback();

		glDisable(GL_RASTERIZER_DISCARD);
	}

	void ParticleContinuousFountain::renderParticles()
	{
		glDisable(GL_DEPTH_TEST);
		m_material->setSubroutine(GL_VERTEX_SHADER, m_renderSub);

		// Setup the other stuff
		m_material->bind();

		glBindVertexArray(m_particleArray[m_drawBuf]);
		glDrawTransformFeedback(GL_POINTS, m_feedback[m_drawBuf]);

		// Swap buffers
		m_drawBuf = 1 - m_drawBuf;

		glEnable(GL_DEPTH_TEST);
	}

	glm::vec3 ParticleContinuousFountain::getVelocityFromCone()
	{
		glm::vec3 v(0.0f);
		glm::vec3 velocity;
		float theta;
		float phi;

		// Pick the direction of the particles
		theta = glm::mix(0.0f, glm::pi<float>() / m_angle, randFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		// Scale to set the magnitude of the velocity
		velocity = mix(m_velocityMin, m_velocityMax, randFloat());
		v = normalize(v) * velocity;

		return v;
	}

	glm::vec3 ParticleContinuousFountain::getVelocityFromCube()
	{
		return mix(m_velocityMin, m_velocityMax, randFloat());
	}

}
