#pragma once
#include "Components\GameComponent.h"
#include "Keyboard.h"
#include "Material.h"

namespace DerydocaEngine::Ext
{

	class ParticleFountain : public Components::GameComponent, Components::SelfRegister<ParticleFountain>
	{
	public:
		GENINSTANCE(ParticleFountain);

		ParticleFountain();
		~ParticleFountain();

		virtual void init();
		virtual void update(float const& deltaTime);
		virtual void preRender();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void render(Rendering::MatrixStack* const& matrixStack);
	private:
		int m_numParticles = 1000;
		Rendering::Material* m_material;
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
