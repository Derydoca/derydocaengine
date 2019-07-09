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

}

#endif
