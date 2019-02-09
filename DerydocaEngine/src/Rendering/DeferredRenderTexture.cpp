#include "EnginePch.h"
#include "DeferredRenderTexture.h"

#include "Rendering\GraphicsAPI.h"

namespace DerydocaEngine::Rendering
{
	DeferredRenderTexture::DeferredRenderTexture() :
		m_depthBuffer(0),
		m_positionBuffer(0),
		m_normalBuffer(0),
		m_colorBuffer(0)
	{
	}

	DeferredRenderTexture::~DeferredRenderTexture()
	{
		deleteBuffers();
	}

	void DeferredRenderTexture::bindAsRenderTexture()
	{
		GraphicsAPI::bindFramebuffer(m_framebuffer);
	}

	void DeferredRenderTexture::initializeBuffer(int width, int height)
	{
		deleteBuffers();

		m_width = width;
		m_height = height;

		// Create and bind the frame buffer object
		GraphicsAPI::createFramebuffers(1, &m_framebuffer);
		GraphicsAPI::bindFramebuffer(m_framebuffer);

		// Create the depth buffer
		glGenRenderbuffers(1, &m_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		// Create the other renderbuffers
		createRenderbuffer(0, GL_RGB32F, m_positionBuffer);
		createRenderbuffer(1, GL_RGB32F, m_normalBuffer);
		createRenderbuffer(2, GL_RGB8, m_colorBuffer);

		// Attach the textures to the framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positionBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colorBuffer, 0);

		GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(4, drawBuffers);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DeferredRenderTexture::createRenderbuffer(int textureUnit, GLenum format, unsigned int & textureId)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexStorage2D(GL_TEXTURE_2D, 1, format, m_width, m_height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}

	void DeferredRenderTexture::deleteBuffers()
	{
		if (m_depthBuffer) GraphicsAPI::deleteRenderBuffer(1, &m_depthBuffer);
		if (m_positionBuffer) GraphicsAPI::deleteRenderBuffer(1, &m_positionBuffer);
		if (m_normalBuffer) GraphicsAPI::deleteRenderBuffer(1, &m_normalBuffer);
		if (m_colorBuffer) GraphicsAPI::deleteRenderBuffer(1, &m_colorBuffer);
		if (m_rendererId) GraphicsAPI::deleteTextures(1, &m_rendererId);
		if (m_framebuffer) GraphicsAPI::deleteFramebuffers(1, &m_framebuffer);
	}

}
