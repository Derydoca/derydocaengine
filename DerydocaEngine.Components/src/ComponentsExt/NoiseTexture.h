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
		SERIALIZE_FUNC_DEFINITIONS;

		NoiseTexture();
		~NoiseTexture();

		virtual void init();

		void generateNoiseTexture();
		int& getWidth() { return m_Size.x; }
		int& getHeight() { return m_Size.y; }
		float& getBaseFrequency() { return m_BaseFrequency; }
		float& getPersistence() { return m_Persistence; }
		bool& getIsPeriodic() { return m_Periodic; }
		bool& getIsSeamless() { return m_Seamless; }

	private:
		int2 m_Size = { 256, 256 };
		std::string m_TextureName = "NoiseTexture";
		float m_BaseFrequency = 4.0;
		float m_Persistence = 1.0;
		bool m_Periodic = false;
		bool m_Seamless = false;

		std::shared_ptr<Rendering::Material> m_Material;
		std::shared_ptr<Rendering::Texture> m_Texture = nullptr;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::NoiseTexture, 0);
