#pragma once
#include <glm\glm.hpp>
#include "Components\GameComponent.h"
#include "Rendering\Texture.h"
#include "Rendering\Material.h"
#include "Resources\ShaderResource.h"
#include "Resources\TextureResource.h"

namespace DerydocaEngine::Ext
{

	class ParticleSystem : public Components::GameComponent, Components::SelfRegister<ParticleSystem>
	{
	public:
		GENINSTANCE(ParticleSystem);
		SERIALIZE_FUNC_DEFINITIONS;

		ParticleSystem();
		~ParticleSystem();

		virtual void init();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		int& getNumParticles() { return m_NumParticles; }
		float& getSize() { return m_Size2; }
		glm::vec3& getVolumeSize() { return m_VolumeSize; }

		void reset();

	private:
		std::shared_ptr<Rendering::Texture> GetTexture() { return std::static_pointer_cast<Rendering::Texture>(m_Texture->getResourceObjectPointer()); }
		std::shared_ptr<Rendering::Shader> GetShader() { return std::static_pointer_cast<Rendering::Shader>(m_Shader->getResourceObjectPointer()); }

	private:
		ResourceRef<Resources::ShaderResource> m_Shader;
		ResourceRef<Resources::TextureResource> m_Texture;
		int m_NumParticles;
		glm::vec3 m_VolumeSize;
		float m_Size2;

		unsigned int m_VAO;
		unsigned int m_VertexArrayBuffers[1];
		glm::vec3* m_ParticleLocations;
		std::shared_ptr<Rendering::Material> m_Material;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::ParticleSystem, 0);
