#pragma once
#include "GameComponent.h"
#include "RenderTexture.h"

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
	Camera* m_postProcessCamera;
	float m_weights[10];
	RenderTexture* m_blurTex;
	RenderTexture* m_blurTex2;

	float gauss(float const& x, float const& sigma2);
	void updateShader();
};

