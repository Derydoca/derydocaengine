#pragma once
#include "Components\GameComponent.h"
#include "Input\Keyboard.h"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class ParticleFountain : public Components::GameComponent, Components::SelfRegister<ParticleFountain>
	{
	public:
		GENINSTANCE(ParticleFountain);

		ParticleFountain();
		~ParticleFountain();

		virtual void init();
		virtual void update(const float deltaTime);
		virtual void preRender();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		float& getVelocityMin() { return m_velocityMin; }
		float& getVelocityMax() { return m_velocityMax; }
		int& getNumParticles() { return m_numParticles; }
		float& getLifetime() { return m_lifetime; }
		float& getAngle() { return m_angle; }

		void resetSimulation();

	private:
		int m_numParticles = 1000;
		std::shared_ptr<Rendering::Material> m_material;
		float m_time = 0.0f;
		float m_lifetime = 5.0f;
		float m_velocityMin = 1.25f;
		float m_velocityMax = 1.5f;
		float m_angle = 6.0f;
		unsigned int m_vao;
		unsigned int m_initVel;
		unsigned int m_startTime;
		Input::Keyboard* m_keyboard;

		void initBuffers();
		float randFloat();
	};

}
