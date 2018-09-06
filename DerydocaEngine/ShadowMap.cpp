#include "ShadowMap.h"
#include "GL\glew.h"
#include "Shader.h"

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::intializeTexture(int const& size)
{
	m_width = size;
	m_height = size;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glGenFramebuffers(1, &m_shadowFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_shadowFramebuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture, 0);
	glDrawBuffer(GL_NONE);
}

void ShadowMap::bind(unsigned int const& unit)
{

}

void ShadowMap::bindAsRenderTexture()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_shadowFramebuffer);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glUniformMatrix4fv()
}
