#include "RenderTexture.h"
#include <cassert>

namespace DerydocaEngine::Rendering
{

	RenderTexture::RenderTexture()
	{

	}

	RenderTexture::RenderTexture(int const& width, int const& height)
	{
		initializeTexture(width, height);
	}

	RenderTexture::~RenderTexture()
	{
		if (m_depthbuffer) glDeleteRenderbuffers(1, &m_depthbuffer);
		if (m_rendererId) glDeleteTextures(1, &m_rendererId);
		if (m_framebuffer) glDeleteFramebuffers(1, &m_framebuffer);
	}

	void RenderTexture::initializeTexture(int const& width, int const& height)
	{
		m_width = width;
		m_height = height;

		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glGenTextures(1, &m_rendererId);
		glBindTexture(GL_TEXTURE_2D, m_rendererId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenRenderbuffers(1, &m_depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_rendererId, 0);
		glDrawBuffers(1, m_drawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("UNABLE TO CREATE RENDER TEXTURE!");
		}
	}

	void RenderTexture::bind(unsigned int const& unit)
	{
		assert(unit >= 0 && unit <= 31);

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_rendererId);
	}

	void RenderTexture::bindAsRenderTexture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	}

	float RenderTexture::getAspectRatio()
	{
		if (m_height != 0) {
			return (float)m_width / (float)m_height;
		}
		return 1;
	}

}
