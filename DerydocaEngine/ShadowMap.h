#pragma once
#include "Texture.h"

class Shader;

class ShadowMap : public Texture
{
public:
	ShadowMap();
	~ShadowMap();

	void bind(unsigned int unit);
	void bindAsRenderTexture();
	void intializeTexture(int size);
private:
	GLuint m_shadowFramebuffer;
};

