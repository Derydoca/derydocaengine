#pragma once
#include "Components\GameComponent.h"
#include "Rendering\RenderTexture.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Ext
{

	class BloomFilter : Components::GameComponent, Components::SelfRegister<BloomFilter>
	{
	public:
		GENINSTANCE(BloomFilter);

		BloomFilter();
		~BloomFilter();

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void update(float const& deltaTime);
	private:
		float m_lumThresh;
		Components::Camera* m_postProcessCamera;
		float m_weights[10];
		std::shared_ptr<Rendering::RenderTexture> m_blurTex;
		std::shared_ptr<Rendering::RenderTexture> m_blurTex2;

		float gauss(float const& x, float const& sigma2);
		void updateShader();
	};

}
