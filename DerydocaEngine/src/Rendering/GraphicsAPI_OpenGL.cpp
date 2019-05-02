#include "EnginePch.h"

#if OPENGL

#include "GraphicsAPI.h"

namespace DerydocaEngine::Rendering
{

	void GraphicsAPI::init()
	{
		GLenum status = glewInit();

		if (status != GLEW_OK) {
			std::cerr << "Unable to initialize OpenGL loader!\n";
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

	int GraphicsAPI::getUniformName(const unsigned int rendererId, const std::string& name)
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

	void GraphicsAPI::setUniform(const int uniformLocation, const glm::vec3& val)
	{
		glUniform3f(uniformLocation, val.x, val.y, val.z);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const glm::vec4& val)
	{
		glUniform4f(uniformLocation, val.x, val.y, val.z, val.w);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const glm::mat3& val)
	{

		glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, &val[0][0]);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const glm::mat4& val)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &val[0][0]);
	}

	void GraphicsAPI::setUniform(const int uniformLocation, const Color& val)
	{
		glUniform4f(uniformLocation, val.r, val.g, val.b, val.a);
	}

}

#endif
