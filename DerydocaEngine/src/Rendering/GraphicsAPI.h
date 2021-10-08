#pragma once
#include "Color.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Rendering
{
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
		static void bindTexture2D(unsigned int unit, unsigned int rendererId);
		static void deleteRenderBuffer(int count, const unsigned int* rendererIds);
		static void deleteTextures(int count, const unsigned int* rendererIds);
		static void deleteFramebuffers(int count, const unsigned int* rendererIds);
		static void deleteUniformBuffer(const unsigned int& rendererId);
		static void createFramebuffers(int count, unsigned int* rendererIds);
		static void createTexture2D(unsigned int* rendererId, int width, int height);
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
		static void setTexture(const int uniformLocation, const int textureUnit, const unsigned int textureType, const unsigned int textureId);
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
		static InternalTextureFormat getInternalTextureFormat(SizedTextureFormat textureFormat);
		static TextureDataType getTextureDataType(SizedTextureFormat textureFormat);

		static std::vector<uint32_t> createDeferredRenderBuffer(const uint16_t width, const uint16_t height, const std::vector<FramebufferDescriptor>& framebufferDescs, uint32_t& renderbuffer);

		static void createForwardRendererBuffer(
			const uint16_t width,
			const uint16_t height,
			SizedTextureFormat format,
			uint32_t& colorBuffer,
			uint32_t& depthBuffer);
	};

}
