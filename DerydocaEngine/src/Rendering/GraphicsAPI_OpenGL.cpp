#include "EnginePch.h"

#if OPENGL

#include "GraphicsAPI.h"
#include "Debug\GLError.h"

namespace DerydocaEngine::Rendering
{
	int GraphicsAPI::USAGE_PATTERN_DYNAMIC_DRAW = GL_DYNAMIC_DRAW;

	void GraphicsAPI::init()
	{
		GLenum status = glewInit();

		if (status != GLEW_OK) {
			D_LOG_CRITICAL("Unable to initialize OpenGL loader!");
		}
	}

	void GraphicsAPI::bindFramebuffer(unsigned int rendererId)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);
	}

	void GraphicsAPI::bindTexture2D(unsigned int unit, unsigned int rendererId)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, rendererId);
	}

	void GraphicsAPI::deleteRenderBuffer(int count, const unsigned int * rendererIds)
	{
		glDeleteRenderbuffers(count, rendererIds);
	}

	void GraphicsAPI::deleteTextures(int count, const unsigned int * rendererIds)
	{
		glDeleteTextures(count, rendererIds);
	}

	void GraphicsAPI::deleteFramebuffers(int count, const unsigned int * rendererIds)
	{
		glDeleteFramebuffers(count, rendererIds);
	}

	void GraphicsAPI::deleteUniformBuffer(const unsigned int& rendererId)
	{
		glDeleteBuffers(1, &rendererId);
	}

	void GraphicsAPI::createUniformBuffer(unsigned int& rendererId, const void* buffer, const size_t size, const int usagePattern)
	{
		glGenBuffers(1, &rendererId);
		glBindBuffer(GL_UNIFORM_BUFFER, rendererId);
		glBufferData(GL_UNIFORM_BUFFER, size, buffer, usagePattern);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GraphicsAPI::createFramebuffers(int count, unsigned int * rendererIds)
	{
		glGenFramebuffers(count, rendererIds);
	}

	void GraphicsAPI::createTexture2D(unsigned int * rendererId, int width, int height)
	{
		glGenTextures(1, rendererId);
		glBindTexture(GL_TEXTURE_2D, *rendererId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}

	void GraphicsAPI::createRenderBuffers(int count, unsigned int * rendererIds)
	{
		glGenRenderbuffers(count, rendererIds);
	}

	void GraphicsAPI::clearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsAPI::clearColorBuffer(Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsAPI::enableTransparancy() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	bool GraphicsAPI::isFramebufferCreated()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	void GraphicsAPI::disableTransparancy()
	{
		glDisable(GL_BLEND);
	}

	void GraphicsAPI::setViewport(std::shared_ptr<Components::Camera> camera, int textureW, int textureH)
	{
		auto displayRect = camera->getDisplayRect();
		glViewport(
			(GLint)(textureW * displayRect.getX()),
			(GLint)(textureH * displayRect.getY()),
			(GLint)(textureW * displayRect.getWidth()),
			(GLint)(textureH * displayRect.getHeight()));
	}

	int GraphicsAPI::getCurrentFramebufferID()
	{
		int boundFrameBufferId;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &boundFrameBufferId);
		return boundFrameBufferId;
	}

	void GraphicsAPI::setTexture(const int uniformLocation, const int textureUnit, const unsigned int textureType, const unsigned int textureId)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(textureType, textureId);
		glUniform1i(uniformLocation, textureUnit);
	}

	int GraphicsAPI::getUniformLocation(const unsigned int rendererId, const std::string& name)
	{
		int uniformName = glGetUniformLocation(rendererId, name.c_str());
		return uniformName;
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const int val)
	{
		glUniform1i(uniformLocation, val);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const float val)
	{
		glUniform1f(uniformLocation, val);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const float v0, const float v1)
	{
		glUniform2f(uniformLocation, v0, v1);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const float v0, const float v1, const float v2)
	{
		glUniform3f(uniformLocation, v0, v1, v2);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const float v0, const float v1, const float v2, const float v3)
	{
		glUniform4f(uniformLocation, v0, v1, v2, v3);
	}

	void GraphicsAPI::setUniformMat3(const int uniformLocation, const float* matrixArrayPointer, int numMatrices)
	{
		glUniformMatrix3fv(uniformLocation, numMatrices, GL_FALSE, matrixArrayPointer);
	}

	void GraphicsAPI::setUniformMat4(const int uniformLocation, const float* matrixArrayPointer, int numMatrices)
	{
		glUniformMatrix4fv(uniformLocation, numMatrices, GL_FALSE, matrixArrayPointer);
	}

	void GraphicsAPI::updateUniformBuffer(const int rendererId, const void * buffer, const size_t size)
	{
		GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, rendererId));
		void* p;
		GL_CHECK(p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
		memcpy(p, buffer, size);
		GL_CHECK(glUnmapBuffer(GL_UNIFORM_BUFFER));
	}

	uint32_t GraphicsAPI::translate(FilterMode filterMode)
	{
		switch (filterMode)
		{
		case DerydocaEngine::Rendering::FilterMode::Nearest:
			return GL_NEAREST;
		case DerydocaEngine::Rendering::FilterMode::Linear:
			return GL_LINEAR;
		default:
			D_LOG_CRITICAL("Unable to translate FilterMode!");
			return -1;
		}
	}

	uint32_t GraphicsAPI::translate(InternalTextureFormat internalTextureFormat)
	{
		switch (internalTextureFormat)
		{
		case DerydocaEngine::Rendering::InternalTextureFormat::R:
			return GL_RED;
		case DerydocaEngine::Rendering::InternalTextureFormat::RG:
			return GL_RG;
		case DerydocaEngine::Rendering::InternalTextureFormat::RGB:
			return GL_RGB;
		case DerydocaEngine::Rendering::InternalTextureFormat::RGBA:
			return GL_RGBA;
		case DerydocaEngine::Rendering::InternalTextureFormat::Depth:
			return GL_DEPTH_COMPONENT;
		case DerydocaEngine::Rendering::InternalTextureFormat::DepthStencil:
			return GL_DEPTH_STENCIL;
		default:
			D_LOG_CRITICAL("Unable to translate InternalTextureFormat!");
			return -1;
		}
	}

	uint32_t GraphicsAPI::translate(SizedTextureFormat textureFormat)
	{
		switch (textureFormat)
		{
		case DerydocaEngine::Rendering::SizedTextureFormat::RGB32F:
			return GL_RGB32F;
		case DerydocaEngine::Rendering::SizedTextureFormat::RGB8:
			return GL_RGB8;
		default:
			D_LOG_CRITICAL("Unable to translate SizedTextureFormat!");
			return -1;
		}
	}

	uint32_t GraphicsAPI::translate(TextureDataType textureFormat)
	{
		switch (textureFormat)
		{
		case DerydocaEngine::Rendering::TextureDataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
		default:
			D_LOG_CRITICAL("Unable to translate TextureDataType!");
			return -1;
		}
	}

	InternalTextureFormat GraphicsAPI::getInternalTextureFormat(SizedTextureFormat textureFormat)
	{
		switch (textureFormat)
		{
		case DerydocaEngine::Rendering::SizedTextureFormat::RGB32F:
			return InternalTextureFormat::RGB;
		case DerydocaEngine::Rendering::SizedTextureFormat::RGB8:
			return InternalTextureFormat::RGB;
		default:
			D_LOG_CRITICAL("Unable to translate SizedTextureFormat to InternalTextureFormat!");
			return InternalTextureFormat::RGB;
		}
	}

	TextureDataType GraphicsAPI::getTextureDataType(SizedTextureFormat textureFormat)
	{
		switch (textureFormat)
		{
		case DerydocaEngine::Rendering::SizedTextureFormat::RGB32F:
			return TextureDataType::Float;
		case DerydocaEngine::Rendering::SizedTextureFormat::RGB8:
			return TextureDataType::UnsignedByte;
		default:
			D_LOG_CRITICAL("Unable to translate SizedTextureFormat to TextureDataType!");
			return TextureDataType::Float;
		}
	}

	std::vector<uint32_t> GraphicsAPI::createDeferredRenderBuffer(const uint16_t width, const uint16_t height, const std::vector<FramebufferDescriptor>& framebufferDescs, uint32_t& renderbuffer)
	{
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		std::vector<uint32_t> framebufferTextures(framebufferDescs.size());
		glGenTextures(framebufferDescs.size(), &framebufferTextures[0]);

		for (int framebufferIdx = 0; framebufferIdx < framebufferDescs.size(); framebufferIdx++)
		{
			auto desc = framebufferDescs[framebufferIdx];

			glActiveTexture(GL_TEXTURE0 + desc.TextureUnit);
			glBindTexture(GL_TEXTURE_2D, framebufferTextures[framebufferIdx]);
			glTexStorage2D(GL_TEXTURE_2D, 1, translate(desc.Format), width, height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, translate(desc.MinFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, translate(desc.MagFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		}

		// Attach the textures to the framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

		std::vector<GLenum> drawBuffers;
		drawBuffers.push_back(GL_NONE);
		for (int framebufferIdx = 0; framebufferIdx < framebufferTextures.size(); framebufferIdx++)
		{
			GLenum attachment = GL_COLOR_ATTACHMENT0 + framebufferIdx;
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, framebufferTextures[framebufferIdx], 0);
			drawBuffers.push_back(attachment);
		}

		glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

		return framebufferTextures;
	}

	void GraphicsAPI::createForwardRendererBuffer(const uint16_t width, const uint16_t height, SizedTextureFormat format, uint32_t& colorBuffer, uint32_t& depthBuffer)
	{
		auto internalTextureFormat = getInternalTextureFormat(format);
		auto textureDataType = getTextureDataType(format);

		// Create the color buffer (Also can be bound to sampler in a shader that takes a single sampler)
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, translate(internalTextureFormat), width, height, 0, translate(internalTextureFormat), translate(textureDataType), 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Create the depth buffer
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffer, 0);
		GLenum colorAttachments[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, colorAttachments);
	}

}

#endif
