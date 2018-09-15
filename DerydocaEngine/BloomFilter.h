#pragma once
#include "GameComponent.h"
#include "RenderTexture.h"
#include "Camera.h"

namespace DerydocaEngine::Ext
{

	class BloomFilter : GameComponent
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
		DerydocaEngine::Components::Camera* m_postProcessCamera;
		float m_weights[10];
		Rendering::RenderTexture* m_blurTex;
		Rendering::RenderTexture* m_blurTex2;

		float gauss(float const& x, float const& sigma2);
		void updateShader();
	};

}
