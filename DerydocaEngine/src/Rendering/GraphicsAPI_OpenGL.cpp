#include "EnginePch.h"

#if OPENGL

#include <GL/glew.h>
#include "GraphicsAPI.h"
#include "Debug\GLError.h"
#include "Shader.h"

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

	void GraphicsAPI::setTexture(const int uniformLocation, const int textureUnit, const TextureType textureType, const unsigned int textureId)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GraphicsAPI::translate(textureType), textureId);
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

	uint32_t GraphicsAPI::translate(ShaderProgramType shaderProgramType)
	{
		switch (shaderProgramType)
		{
		case DerydocaEngine::Rendering::ShaderProgramType::VertexShader:
			return GL_VERTEX_SHADER;
		case DerydocaEngine::Rendering::ShaderProgramType::TesselationControlShader:
			return GL_TESS_CONTROL_SHADER;
		case DerydocaEngine::Rendering::ShaderProgramType::TesselationEvaluationShader:
			return GL_TESS_EVALUATION_SHADER;
		case DerydocaEngine::Rendering::ShaderProgramType::GeometryShader:
			return GL_GEOMETRY_SHADER;
		case DerydocaEngine::Rendering::ShaderProgramType::FragmentShader:
			return GL_FRAGMENT_SHADER;
		default:
			D_LOG_CRITICAL("Unable to translate ShaderProgramType!");
			return -1;
		}
	}

	uint32_t GraphicsAPI::translate(ShaderValidationFlag shaderValidationFlag)
	{
		switch (shaderValidationFlag)
		{
		case DerydocaEngine::Rendering::ShaderValidationFlag::LinkStatus:
			return GL_LINK_STATUS;
		case DerydocaEngine::Rendering::ShaderValidationFlag::ValidateStatus:
			return GL_VALIDATE_STATUS;
		case DerydocaEngine::Rendering::ShaderValidationFlag::CompileStatus:
			return GL_COMPILE_STATUS;
		default:
			D_LOG_CRITICAL("Unable to translate ShaderValidationFlag!");
			return -1;
		}
	}

	uint32_t GraphicsAPI::translate(TextureType textureType)
	{
		switch (textureType)
		{
		case DerydocaEngine::Rendering::TextureType::Texture2D:
			return GL_TEXTURE_2D;
		case DerydocaEngine::Rendering::TextureType::Texture2DArray:
			return GL_TEXTURE_2D_ARRAY;
		case DerydocaEngine::Rendering::TextureType::Texture3D:
			return GL_TEXTURE_3D;
		case DerydocaEngine::Rendering::TextureType::Cubemap:
			return GL_TEXTURE_CUBE_MAP;
		case DerydocaEngine::Rendering::TextureType::CubemapArray:
			return GL_TEXTURE_CUBE_MAP_ARRAY;
		default:
			D_LOG_CRITICAL("Unable to translate TextureType!");
			return -1;
		}
	}

	uint32_t GraphicsAPI::translate(DataType dataType)
	{
		switch (dataType)
		{
		case DerydocaEngine::Rendering::DataType::Float:
			return GL_FLOAT;
		case DerydocaEngine::Rendering::DataType::Int:
			return GL_INT;
		case DerydocaEngine::Rendering::DataType::UnsignedInt:
			return GL_UNSIGNED_INT;
		default:
			D_LOG_CRITICAL("Unable to translate DataType!");
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

	size_t GraphicsAPI::getSize(DataType dataType)
	{
		switch (dataType)
		{
		case DerydocaEngine::Rendering::DataType::Float:
			return sizeof(float);
		case DerydocaEngine::Rendering::DataType::Int:
			return sizeof(int);
		case DerydocaEngine::Rendering::DataType::UnsignedInt:
			return sizeof(unsigned int);
		default:
			D_LOG_CRITICAL("Unable to get size of DataType!");
			return 0;
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

	bool GraphicsAPI::validateShader(uint32_t shader, ShaderValidationFlag flag, const std::string& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = { 0 };

		glGetShaderiv(shader, translate(flag), &success);

		if (success == GL_FALSE) {
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

			D_LOG_ERROR("{}:\n{}", errorMessage, error);

			return false;
		}

		return true;
	}

	bool GraphicsAPI::validateShaderProgram(uint32_t shaderProgram, ShaderValidationFlag flag, const std::string& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = { 0 };

		glGetProgramiv(shaderProgram, translate(flag), &success);

		if (success == GL_FALSE) {
			glGetProgramInfoLog(shaderProgram, sizeof(error), NULL, error);

			D_LOG_ERROR("{}:\n{}", errorMessage, error);

			return false;
		}

		return true;
	}

	uint32_t GraphicsAPI::createShader(const std::string& shaderProgramSource, ShaderProgramType shaderProgramType)
	{
		GLuint shader = glCreateShader(translate(shaderProgramType));

		if (shader == 0)
		{
			D_LOG_ERROR("Shader creation failed!");
		}

		const GLchar* shaderSourceStrings[1];
		GLint shaderSourceStringLengths[1];

		shaderSourceStrings[0] = shaderProgramSource.c_str();
		shaderSourceStringLengths[0] = static_cast<GLint>(shaderProgramSource.length());

		glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
		glCompileShader(shader);

		if (!validateShader(shader, ShaderValidationFlag::CompileStatus, "Error: Shader compilation failed: "))
		{
			return Shader::getErrorShaderID();
		}

		return shader;
	}

	uint32_t GraphicsAPI::createShaderProgram(uint32_t vertexShader, uint32_t tessControlShader, uint32_t tessEvalShader, uint32_t geometryShader, uint32_t fragmentShader, std::vector<ShaderAttribute> shaderAttributes, std::vector<ShaderAttribute> outputAttributes)
	{
		// Create the program
		uint32_t shaderProgram = glCreateProgram();
		if (0 == shaderProgram)
		{
			D_LOG_ERROR("Error creating program object.");
		}

		if (vertexShader > 0) glAttachShader(shaderProgram, vertexShader);
		if (tessControlShader > 0) glAttachShader(shaderProgram, tessControlShader);
		if (tessEvalShader > 0) glAttachShader(shaderProgram, tessEvalShader);
		if (geometryShader > 0) glAttachShader(shaderProgram, geometryShader);
		if (fragmentShader > 0) glAttachShader(shaderProgram, fragmentShader);

		// Get the vertex attribute locations
		for (auto attribute : shaderAttributes)
		{
			glBindAttribLocation(shaderProgram, attribute.index, attribute.name.c_str());
		}

		// Bind the output buffers
		for (auto attribute : outputAttributes)
		{
			glBindFragDataLocation(shaderProgram, attribute.index, attribute.name.c_str());
		}

		// Link the program
		glLinkProgram(shaderProgram);
		if (!validateShaderProgram(shaderProgram, ShaderValidationFlag::LinkStatus, "Error: Program linking failed"))
		{
			glDeleteProgram(shaderProgram);
			return -1;
		}

		glValidateProgram(shaderProgram);
		if (!validateShaderProgram(shaderProgram, ShaderValidationFlag::ValidateStatus, "Error: Program is invalid"))
		{
			glDeleteProgram(shaderProgram);
			return -1;
		}

		return shaderProgram;
	}

	void GraphicsAPI::deleteShader(const uint32_t shaderProgram, const uint32_t shader)
	{
		glDetachShader(shaderProgram, shader);
		glDeleteShader(shader);
	}

	void GraphicsAPI::deleteShaderProgram(const uint32_t shaderProgram)
	{
		glDeleteProgram(shaderProgram);
	}

	void GraphicsAPI::bindShaderProgram(const uint32_t shaderProgram)
	{
		glUseProgram(shaderProgram);
	}
	void GraphicsAPI::bindUniformBuffer(const uint32_t shaderProgram, const std::string& name, const uint32_t uniformBufferId)
	{
		unsigned int blockIndex = glGetUniformBlockIndex(shaderProgram, name.c_str());
		if (blockIndex != GL_INVALID_INDEX)
		{
			GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uniformBufferId));
		}
	}
	void GraphicsAPI::uploadVertexBuffer(uint32_t gpuBuffer, DataType dataType, int8_t stride, const void* buffer, size_t count, uint32_t attributeIndex)
	{
		glBindBuffer(GL_ARRAY_BUFFER, gpuBuffer);
		glBufferData(GL_ARRAY_BUFFER, count * getSize(dataType) * stride, buffer, GL_STATIC_DRAW);
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, stride, translate(dataType), GL_FALSE, 0, 0);
	}
	void GraphicsAPI::deleteVertexArray(uint32_t& vertexArrayObject)
	{
		glDeleteVertexArrays(1, &vertexArrayObject);
	}
	void GraphicsAPI::drawBoundVertexArray(const size_t numIndices)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, static_cast<int>(numIndices), GL_UNSIGNED_INT, 0, 0);
	}

	void GraphicsAPI::uploadIndexBuffer(const uint32_t& indexBuffer, const size_t numIndices, void* cpuIndexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), cpuIndexBuffer, GL_STATIC_DRAW);
	}

	void GraphicsAPI::bindVertexArray(const uint32_t vertexArrayObject)
	{
		glBindVertexArray(vertexArrayObject);
	}

	void GraphicsAPI::generateVertexArray(uint32_t& vertexArrayObject)
	{
		glGenVertexArrays(1, &vertexArrayObject);
	}

	void GraphicsAPI::generateBuffers(size_t numBuffers, uint32_t& buffers)
	{
		glGenBuffers(numBuffers, &buffers);
	}

	void GraphicsAPI::setDepthTest(const bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
}

#endif
