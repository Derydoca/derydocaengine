#pragma once
#include "Color.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Rendering
{
	enum class ShaderProgramType : uint8_t
	{
		VertexShader,
		TesselationControlShader,
		TesselationEvaluationShader,
		GeometryShader,
		FragmentShader
	};

	enum class ShaderValidationFlag : uint8_t
	{
		LinkStatus,
		ValidateStatus,
		CompileStatus
	};

	enum class InternalTextureFormat : uint8_t
	{
		R,
		RG,
		RGB,
		RGBA,
		Depth,
		DepthStencil
	};

	enum class SizedTextureFormat : uint8_t
	{
		RGB32F,
		RGB8
	};

	enum class TextureDataType : uint8_t
	{
		UnsignedByte,
		Float
	};

	enum class FilterMode : uint8_t
	{
		Nearest,
		Linear
	};

	enum class TextureType : uint8_t
	{
		Texture2D,
		Texture2DArray,
		Texture3D,
		Cubemap,
		CubemapArray
	};

	enum class DataType : uint8_t
	{
		Float,
		Int,
		UnsignedInt
	};

	struct ShaderAttribute
	{
		int index;
		std::string name;
	};

	struct FramebufferDescriptor
	{
		uint8_t TextureUnit;
		SizedTextureFormat Format;
		FilterMode MinFilter;
		FilterMode MagFilter;
	};

	class GraphicsAPI
	{
	public:
		static int USAGE_PATTERN_DYNAMIC_DRAW;

		static void init();
		static void bindFramebuffer(unsigned int rendererId);
		static void bindTexture2D(const uint32_t rendererId, const TextureType textureType, const uint16_t unit);
		static void bindTexture2D(unsigned int unit, unsigned int rendererId); // Deprecated
		static void deleteRenderBuffer(int count, const unsigned int* rendererIds);
		static void deleteTextures(int count, const unsigned int* rendererIds);
		static void deleteFramebuffers(int count, const unsigned int* rendererIds);
		static void deleteUniformBuffer(const unsigned int& rendererId);
		static void createFramebuffers(int count, unsigned int* rendererIds);
		static void createTexture2D(unsigned int* rendererId, int width, int height); // Deprecated
		static void createTexture2D(uint32_t& rendererId, const TextureType textureType, const InternalTextureFormat textureFormat, const uint32_t width, const uint32_t height, bool createMipmaps, const void* imageData);
		static void createRenderBuffers(int count, unsigned int* rendererIds);
		static void createUniformBuffer(unsigned int& rendererId, const void* buffer, const size_t size, const int usagePattern);
		static void clearDepthBuffer();
		static void clearColorBuffer(Color color);
		static void disableTransparancy();
		static void enableTransparancy();
		static bool isFramebufferCreated();
		static void setViewport(std::shared_ptr<Components::Camera> camera, int textureW, int textureH);
		static int getCurrentFramebufferID();
		static int getUniformLocation(const unsigned int rendererId, const std::string& name);
		static void setTexture(const int uniformLocation, const int textureUnit, const TextureType textureType, const unsigned int textureId);
		static void setUniform(const int uniformLocation, const int val);
		static void setUniform(const int uniformLocation, const float val);
		static void setUniform(const int uniformLocation, const float v0, const float v1);
		static void setUniform(const int uniformLocation, const float v0, const float v1, const float v2);
		static void setUniform(const int uniformLocation, const float v0, const float v1, const float v2, const float v3);
		static void setUniformMat3(const int uniformLocation, const float* matrixArrayPointer, int numMatrices);
		static void setUniformMat4(const int uniformLocation, const float* matrixArrayPointer, int numMatrices);
		static void updateUniformBuffer(const int rendererId, const void* buffer, const size_t size);

		static uint32_t translate(FilterMode filterMode);
		static uint32_t translate(InternalTextureFormat internalTextureFormat);
		static uint32_t translate(SizedTextureFormat textureFormat);
		static uint32_t translate(TextureDataType textureFormat);
		static uint32_t translate(ShaderProgramType shaderProgramType);
		static uint32_t translate(ShaderValidationFlag shaderValidationFlag);
		static uint32_t translate(TextureType textureType);
		static uint32_t translate(DataType dataType);
		static InternalTextureFormat getInternalTextureFormat(SizedTextureFormat textureFormat);
		static TextureDataType getTextureDataType(SizedTextureFormat textureFormat);
		static size_t getSize(DataType dataType);

		static std::vector<uint32_t> createDeferredRenderBuffer(const uint16_t width, const uint16_t height, const std::vector<FramebufferDescriptor>& framebufferDescs, uint32_t& renderbuffer);

		static void createForwardRendererBuffer(
			const uint16_t width,
			const uint16_t height,
			SizedTextureFormat format,
			uint32_t& colorBuffer,
			uint32_t& depthBuffer);

		static bool validateShader(uint32_t shader, ShaderValidationFlag flag, const std::string& errorMessage);
		static bool validateShaderProgram(uint32_t shader, ShaderValidationFlag flag, const std::string& errorMessage);

		static uint32_t createShader(const std::string& shaderProgramSource, ShaderProgramType shaderProgramType);
		static uint32_t createShaderProgram(uint32_t vertexShader, uint32_t tessControlShader, uint32_t tessEvalShader, uint32_t geometryShader, uint32_t fragmentShader, std::vector<ShaderAttribute> shaderAttributes, std::vector<ShaderAttribute> outputAttributes);
		static void deleteShader(const uint32_t shaderProgram, const uint32_t shader);
		static void deleteShaderProgram(const uint32_t shaderProgram);
		static void bindShaderProgram(const uint32_t shaderProgram);
		static void bindUniformBuffer(const uint32_t shaderProgram, const std::string& name, const uint32_t uniformBufferId);

		static void uploadVertexBuffer(uint32_t gpuBuffer, DataType dataType, int8_t stride, const void* buffer, size_t count, uint32_t attributeIndex);

		static void deleteVertexArray(uint32_t& vertexArrayObject);
		static void drawBoundVertexArray(const size_t numIndices);
		static void uploadIndexBuffer(const uint32_t& indexBuffer, const size_t numIndices, void* cpuIndexBuffer);
		static void bindVertexArray(const uint32_t vertexArrayObject);
		static void generateVertexArray(uint32_t& vertexArrayObject);
		static void generateBuffers(size_t numBuffers, uint32_t& buffers);

		static void setDepthTest(const bool enabled);

		static void generateCubemap(uint32_t& rendererId, const std::array<char*, 6> cubemapSourceImageData, const uint32_t width, const uint32_t height, const InternalTextureFormat textureFormat, const TextureDataType textureDataType, const bool generateMipmaps);

		//static void uploadVertexBuffer(uint32_t gpuBuffer, int8_t stride, const void* buffer, size_t count, uint32_t attributeIndex)
		//{
		//	//glBindBuffer(GL_ARRAY_BUFFER, gpuBuffer);
		//	//glBufferData(GL_ARRAY_BUFFER, count * getSize(dataType) * stride, buffer, GL_STATIC_DRAW);
		//	//glEnableVertexAttribArray(attributeIndex);
		//	//glVertexAttribPointer(attributeIndex, stride, translate(dataType), GL_FALSE, 0, 0);

		//	glBindBuffer(GL_ARRAY_BUFFER, gpuBuffer);
		//	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Animation::VertexBoneWeights), buffer, GL_STATIC_DRAW);
		//	glEnableVertexAttribArray(attributeIndex);
		//	glVertexAttribIPointer(attributeIndex, Animation::MAX_BONES, GL_UNSIGNED_INT, sizeof(Animation::VertexBoneWeights), 0);
		//}
	};

}
