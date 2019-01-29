#include "EnginePch.h"
#include "Rendering\RenderTexture.h"

#include <GL/glew.h>
#include <cassert>
#include "GraphicsAPI.h"

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
		GraphicsAPI::deleteRenderBuffer(1, &m_depthbuffer);
		GraphicsAPI::deleteTextures(1, &m_rendererId);
		GraphicsAPI::deleteFramebuffers(1, &m_framebuffer);
	}

	void RenderTexture::initializeTexture(int const& width, int const& height)
	{
		m_width = width;
		m_height = height;

		GraphicsAPI::createFramebuffers(1, &m_framebuffer);
		GraphicsAPI::bindFramebuffer(m_framebuffer);
		GraphicsAPI::createTexture2D(&m_rendererId, m_width, m_height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		GraphicsAPI::createRenderBuffers(1, &m_depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_rendererId, 0);
		GLenum colorAttachments[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, colorAttachments);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("UNABLE TO CREATE RENDER TEXTURE!");
		}
	}

	void RenderTexture::bind(unsigned int const& unit)
	{
		assert(unit >= 0 && unit <= 31);

		GraphicsAPI::bindTexture2D(unit, m_rendererId);
	}

	void RenderTexture::bindAsRenderTexture()
	{
		GraphicsAPI::bindFramebuffer(m_framebuffer);
	}

	float RenderTexture::getAspectRatio()
	{
		if (m_height != 0) {
			return (float)m_width / (float)m_height;
		}
		return 1;
	}

}
