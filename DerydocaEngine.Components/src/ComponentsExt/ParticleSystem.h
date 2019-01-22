#pragma once
#include <glm\glm.hpp>
#include "Components\GameComponent.h"
#include "Rendering\Texture.h"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class ParticleSystem : public Components::GameComponent, Components::SelfRegister<ParticleSystem>
	{
	public:
		GENINSTANCE(ParticleSystem);

		ParticleSystem();
		~ParticleSystem();

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		int& getNumParticles() { return m_numParticles; }
		float& getSize() { return m_size2; }
		glm::vec3& getVolumeSize() { return m_volumeSize; }

		void reset();

	private:
		int m_numParticles;
		float m_size2;
		unsigned int m_vao;
		unsigned int m_vertexArrayBuffers[1];
		glm::vec3 m_volumeSize;
		glm::vec3* m_particleLocations;
		std::shared_ptr<Rendering::Texture> m_texture;
		std::shared_ptr<Rendering::Material> m_material;
	};

}
