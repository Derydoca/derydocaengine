#include "EnginePch.h"
#include "Rendering\RenderTexture.h"

#include <GL/glew.h>
#include <cassert>
#include "GraphicsAPI.h"

namespace DerydocaEngine::Rendering
{

	RenderTexture::RenderTexture() :
		RenderTexture(RenderingMode::Forward)
	{
	}

	RenderTexture::RenderTexture(RenderingMode renderingMode) :
		m_renderingMode(renderingMode),
		m_framebuffer(0),
		m_renderBufferIds()
	{
		if (renderingMode == RenderingMode::Forward)
		{
			m_renderBufferIds.resize(1);
		}
		else
		{
			m_renderBufferIds.resize(4);
		}
	}

	RenderTexture::~RenderTexture()
	{
		destroyGraphicsApiObjects();
	}

	void RenderTexture::initializeTexture(int const& width, int const& height)
	{
		destroyGraphicsApiObjects();

		m_width = width;
		m_height = height;

		GraphicsAPI::createFramebuffers(1, &m_framebuffer);
		GraphicsAPI::bindFramebuffer(m_framebuffer);

		if (m_renderingMode == RenderingMode::Forward)
		{
			createRenderbuffers_Forward();
		}
		else
		{
			createRenderbuffers_Deferred();
		}

		auto framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
			printf("UNABLE TO CREATE RENDER TEXTURE!");
		}
	}

	void RenderTexture::createRenderbuffer(int textureUnit, GLenum format, unsigned int & textureId)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexStorage2D(GL_TEXTURE_2D, 1, format, m_width, m_height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}

	void RenderTexture::createRenderbuffers_Forward()
	{
		// Create the color buffer (Also can be bound to sampler in a shader that takes a single sampler)
		GraphicsAPI::createTexture2D(&m_rendererId, m_width, m_height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Create the depth buffer
		GraphicsAPI::createRenderBuffers(1, &m_renderBufferIds[0]);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferIds[0]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferIds[0]);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_rendererId, 0);
		GLenum colorAttachments[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, colorAttachments);
	}

	void RenderTexture::createRenderbuffers_Deferred()
	{
		// Create the depth buffer
		glGenRenderbuffers(1, &m_renderBufferIds[0]);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferIds[0]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

		// Create the other renderbuffers
		createRenderbuffer(0, GL_RGB32F, m_renderBufferIds[1]);
		createRenderbuffer(1, GL_RGB32F, m_renderBufferIds[2]);
		createRenderbuffer(2, GL_RGB8, m_renderBufferIds[3]);

		// Attach the textures to the framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferIds[0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderBufferIds[1], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_renderBufferIds[2], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_renderBufferIds[3], 0);

		GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(4, drawBuffers);
	}

	void RenderTexture::destroyGraphicsApiObjects()
	{
		for (auto bufferId : m_renderBufferIds)
		{
			if (bufferId)
			{
				GraphicsAPI::deleteRenderBuffer(1, &bufferId);
			}
		}

		if (m_rendererId) GraphicsAPI::deleteTextures(1, &m_rendererId);
		if (m_framebuffer) GraphicsAPI::deleteFramebuffers(1, &m_framebuffer);
	}

	void RenderTexture::bind(unsigned int const& unit)
	{
		assert(unit >= 0 && unit <= 31);

		GraphicsAPI::bindTexture2D(unit, m_rendererId);
	}

	void RenderTexture::bindDeferredTextures(std::shared_ptr<Rendering::Shader> shader)
	{
		shader->setTexture("PositionTex", 0, GL_TEXTURE_2D, m_renderBufferIds[1]);
		shader->setTexture("NormalTex", 1, GL_TEXTURE_2D, m_renderBufferIds[2]);
		shader->setTexture("ColorTex", 2, GL_TEXTURE_2D, m_renderBufferIds[3]);
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
