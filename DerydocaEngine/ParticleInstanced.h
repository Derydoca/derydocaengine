#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Keyboard.h"

namespace DerydocaEngine::Ext
{

	class ParticleInstanced : public GameComponent
	{
	public:
		GENINSTANCE(ParticleInstanced);

		ParticleInstanced();
		~ParticleInstanced();

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
		glm::vec3 m_gravity = glm::vec3(0.0f, -0.2f, 0.0f);
		GLuint m_initVel;
		GLuint m_startTime;
		Keyboard* m_keyboard;
		Rendering::Mesh* m_mesh;

		void initBuffers();
		float randFloat();
	};

}
