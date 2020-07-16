#pragma once
#include "Components\GameComponent.h"
#include "Rendering\RenderTexture.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Ext
{

	class BloomFilter : public Components::GameComponent, Components::SelfRegister<BloomFilter>
	{
	public:
		GENINSTANCE(BloomFilter);
		SERIALIZE_FUNC_DEFINITIONS;

		BloomFilter();
		~BloomFilter();

		virtual void init();
		virtual void update(const float deltaTime);

		float& GetThreshold() { return m_Threshold; }

	private:
		float m_Threshold;

		std::shared_ptr<Components::Camera> m_PostProcessCamera;
		std::vector<float> m_Weights;
		std::shared_ptr<Rendering::RenderTexture> m_BlurTex;
		std::shared_ptr<Rendering::RenderTexture> m_BlurTex2;

		float gauss(float const& x, float const& sigma2);
		void updateShader();
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::BloomFilter, 0);
