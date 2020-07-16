#pragma once
#include "Components\GameComponent.h"
#include "Input\Keyboard.h"
#include "Rendering\Material.h"
#include "Resources\ShaderResource.h"
#include "Resources\TextureResource.h"

namespace DerydocaEngine::Ext
{

	class ParticleFountain : public Components::GameComponent, Components::SelfRegister<ParticleFountain>
	{
	public:
		GENINSTANCE(ParticleFountain);
		SERIALIZE_FUNC_DEFINITIONS;

		ParticleFountain();
		~ParticleFountain();

		virtual void init();
		virtual void update(const float deltaTime);
		virtual void preRender();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		float& getVelocityMin() { return m_VelocityMin; }
		float& getVelocityMax() { return m_VelocityMax; }
		int& getNumParticles() { return m_NumParticles; }
		float& getLifetime() { return m_Lifetime; }
		float& getAngle() { return m_Angle; }

		void resetSimulation();

	private:
		int m_NumParticles = 1000;
		float m_VelocityMin = 1.25f;
		float m_VelocityMax = 1.5f;
		float m_Angle = 6.0f;
		float m_Lifetime = 5.0f;
		ResourceRef<Resources::ShaderResource> m_Shader;
		ResourceRef<Resources::TextureResource> m_Texture;

		std::shared_ptr<Rendering::Material> m_Material;
		float m_Time = 0.0f;
		unsigned int m_VAO;
		unsigned int m_InitVel;
		unsigned int m_StartTime;
		Input::Keyboard* m_Keyboard;

		void initBuffers();
		float randFloat();
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::ParticleFountain, 0);
