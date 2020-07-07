#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"
#include "Resources\ShaderResource.h"
#include "Resources\TextureResource.h"

class YAML::Node;

namespace DerydocaEngine::Ext
{

	class ParticleContinuousFountain : public Components::GameComponent, Components::SelfRegister<ParticleContinuousFountain>
	{
	public:
		enum ParticleEmitterType
		{
			Point = 0,
			Cube = 1,
			Cone = 2
		};

		GENINSTANCE(ParticleContinuousFountain);
		SERIALIZE_FUNC_DEFINITIONS;

		ParticleContinuousFountain();
		~ParticleContinuousFountain();

		virtual void init();
		virtual void update(const float deltaTime);
		virtual void preRender();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		glm::vec3& getVelocityMin() { return m_VelocityMin; }
		glm::vec3& getVelocityMax() { return m_VelocityMax; }
		int& getNumParticles() { return m_NumParticles; }
		float& getLifetime() { return m_Lifetime; }
		float& getAngle() { return m_Angle; }
		glm::vec3 getAcceleration() { return m_Acceleration; }
		glm::vec3 getEmitterSize() { return m_EmitterSize; }
		float& getParticleSizeMin() { return m_ParticleSizeMin; }
		float& getParticleSizeMax() { return m_ParticleSizeMax; }
		float& getSpawnRatePerSecond() { return m_SpawnRatePerSecond; }

		void resetSimulation();

	private:
		int m_NumParticles = 1000;
		glm::vec3 m_VelocityMin = glm::vec3(1.25f);
		glm::vec3 m_VelocityMax = glm::vec3(1.5f);
		float m_Angle = 6.0f;
		float m_Lifetime = 5.0f;
		ParticleEmitterType m_EmitterType;
		glm::vec3 m_EmitterSize = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 m_Acceleration = glm::vec3(0.0f, -0.4f, 0.0f);
		float m_ParticleSizeMin = 10.0f;
		float m_ParticleSizeMax = 10.0f;
		float m_SpawnRatePerSecond = 100.0f;
		ResourceRef<Resources::ShaderResource> m_Shader;
		ResourceRef<Resources::TextureResource> m_Texture;

		std::shared_ptr<Rendering::Material> m_Material;
		float m_Time = 0.0f;
		float m_LastDeltaTime = 0.0f;
		unsigned int m_PosBuf[2];
		unsigned int m_VelBuf[2];
		unsigned int m_StartTime[2];
		unsigned int m_InitVel;
		unsigned int m_InitPos;
		unsigned int m_ParticleArray[2];
		unsigned int m_Feedback[2];
		unsigned int m_DrawBuf = 1;
		unsigned int m_UpdateSub;
		unsigned int m_RenderSub;
		std::shared_ptr<Components::Transform> m_Trans;

		void initBuffers();
		float randFloat();
		void updateParticlePositions(float const& deltaTime);
		void renderParticles();
		glm::vec3 getVelocityFromCone();
		glm::vec3 getVelocityFromCube();
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::ParticleContinuousFountain);
