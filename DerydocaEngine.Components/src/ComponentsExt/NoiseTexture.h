#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"

class Texture;

namespace DerydocaEngine::Ext
{

	class NoiseTexture : public Components::GameComponent, Components::SelfRegister<NoiseTexture>
	{
	public:
		GENINSTANCE(NoiseTexture);

		NoiseTexture();
		~NoiseTexture();

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);

		void generateNoiseTexture(float const& baseFreq, float const& persistence, bool const& periodic, bool const& seamless);
	private:
		int m_width = 256;
		int m_height = 256;
		std::string m_textureName = "NoiseTexture";
		std::shared_ptr<Rendering::Material> m_material;
		std::shared_ptr<Rendering::Texture> m_texture = nullptr;
		float m_baseFrequency = 4.0;
		float m_persistence = 1.0;
		bool m_periodic = false;
		bool m_seamless = false;
	};

}
