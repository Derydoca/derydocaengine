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
			GraphicsAPI::createForwardRendererBuffer(m_width, m_height, SizedTextureFormat::RGB8, m_rendererId, m_renderBufferIds[0]);
		}
		else
		{
			std::vector<FramebufferDescriptor> framebufferDescs = {
				{0, SizedTextureFormat::RGB32F, FilterMode::Nearest, FilterMode::Nearest},
				{1, SizedTextureFormat::RGB32F, FilterMode::Nearest, FilterMode::Nearest},
				{2, SizedTextureFormat::RGB8, FilterMode::Nearest, FilterMode::Nearest}
			};
			auto framebuffers = GraphicsAPI::createDeferredRenderBuffer(m_width, m_height, framebufferDescs, m_renderBufferIds[0]);
			for (size_t i = 0; i < framebuffers.size(); i++)
			{
				m_renderBufferIds[i + 1] = framebuffers[i];
			}
		}

		if (!GraphicsAPI::isFramebufferCreated())
		{
			D_LOG_ERROR("UNABLE TO CREATE RENDER TEXTURE!");
		}
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
