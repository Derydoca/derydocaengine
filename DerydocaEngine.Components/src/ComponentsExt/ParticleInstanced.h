#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Mesh.h"
#include "Input\Keyboard.h"
#include "Resources\ShaderResource.h"
#include "Resources\MeshResource.h"

namespace DerydocaEngine::Ext
{

	class ParticleInstanced : public Components::GameComponent, Components::SelfRegister<ParticleInstanced>
	{
	public:
		GENINSTANCE(ParticleInstanced);
		SERIALIZE_FUNC_DEFINITIONS;

		ParticleInstanced();
		~ParticleInstanced();

		virtual void init();
		virtual void update(const float deltaTime);
		virtual void preRender();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		float& getVelocityMin() { return m_VelocityMin; }
		float& getVelocityMax() { return m_VelocityMax; }
		int& getNumParticles() { return m_NumParticles; }
		float& getLifetime() { return m_Lifetime; }
		float& getAngle() { return m_Angle; }
		glm::vec3 getGravity() { return m_Gravity; }

		void resetSimulation();

	private:
		int m_NumParticles = 1000;
		float m_VelocityMin = 1.25f;
		float m_VelocityMax = 1.5f;
		float m_Angle = 6.0f;
		ResourceRef<Resources::ShaderResource> m_Shader;
		ResourceRef<Resources::MeshResource> m_Mesh;

		std::shared_ptr<Rendering::Material> m_Material;
		float m_Time = 0.0f;
		float m_Lifetime = 5.0f;
		glm::vec3 m_Gravity = glm::vec3(0.0f, -0.2f, 0.0f);
		unsigned int m_InitVel;
		unsigned int m_StartTime;
		Input::Keyboard* m_Keyboard;

		void initBuffers();
		float randFloat();
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::ParticleInstanced, 0);
