#include "RenderTexture.h"
#include <iostream>
#include <cassert>

RenderTexture::RenderTexture()
{

}

RenderTexture::RenderTexture(int width, int height)
{
	initializeTexture(width, height);
}

RenderTexture::~RenderTexture()
{
	if(m_depthbuffer) glDeleteRenderbuffers(1, &m_depthbuffer);
	if(m_texture) glDeleteTextures(1, &m_texture);
	if(m_framebuffer) glDeleteFramebuffers(1, &m_framebuffer);
}

void RenderTexture::initializeTexture(int width, int height)
{
	m_width = width;
	m_height = height;

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &m_depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture, 0);
	glDrawBuffers(1, m_drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("UNABLE TO CREATE RENDER TEXTURE!");
	}
}

void RenderTexture::bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void RenderTexture::bindAsRenderTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

float RenderTexture::getAspectRatio()
{
	if (m_height != 0) {
		return m_width / m_height;
	}
	return 1;
}
