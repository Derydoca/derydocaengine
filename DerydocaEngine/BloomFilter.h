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
	virtual void deserialize(YAML::Node compNode);
	virtual void update(float deltaTime);
private:
	float m_lumThresh;
	Camera* m_postProcessCamera;
	float m_weights[10];
	RenderTexture* m_blurTex;
	RenderTexture* m_blurTex2;

	float gauss(float x, float sigma2);
	void updateShader();
};

