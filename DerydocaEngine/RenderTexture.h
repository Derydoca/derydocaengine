#pragma once
#include <GL\glew.h>
#include "Texture.h"

class RenderTexture : public Texture
{
public:
	RenderTexture();
	RenderTexture(int const& width, int const& height);
	~RenderTexture();

	void bind(unsigned int const& unit);
	void bindAsRenderTexture();
	float getAspectRatio();
	void initializeTexture(int const& width, int const& height);
private:
	GLuint m_framebuffer;
	GLuint m_depthbuffer;

	GLenum m_drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
};
