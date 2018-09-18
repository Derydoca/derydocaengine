#pragma once
#include <glm\glm.hpp>
#include "GameComponent.h"
#include "Texture.h"
//#include "Shader.h"
#include "Material.h"

namespace DerydocaEngine::Ext
{

	class ParticleSystem : public GameComponent
	{
	public:
		GENINSTANCE(ParticleSystem);

		ParticleSystem();
		~ParticleSystem();

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		void render(Rendering::MatrixStack* const& matrixStack);
	private:
		unsigned int m_numParticles;
		float m_size2;
		GLuint m_vao;
		GLuint m_vertexArrayBuffers[1];
		glm::vec3 m_volumeSize;
		glm::vec3* m_particleLocations;
		Rendering::Texture* m_texture;
		Rendering::Material* m_material;
	};

}
