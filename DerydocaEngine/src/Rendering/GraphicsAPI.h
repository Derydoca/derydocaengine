#pragma once
#include "Color.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Rendering
{

	class GraphicsAPI
	{
	public:
		static void init();
		static void bindFramebuffer(unsigned int rendererId);
		static void bindTexture2D(unsigned int unit, unsigned int rendererId);
		static void deleteRenderBuffer(int count, const unsigned int* rendererIds);
		static void deleteTextures(int count, const unsigned int* rendererIds);
		static void deleteFramebuffers(int count, const unsigned int* rendererIds);
		static void createFramebuffers(int count, unsigned int* rendererIds);
		static void createTexture2D(unsigned int* rendererId, int width, int height);
		static void createRenderBuffers(int count, unsigned int* rendererIds);
		static void clearDepthBuffer();
		static void clearColorBuffer(Color color);
		static void setViewport(std::shared_ptr<Components::Camera> camera, int textureW, int textureH);
		static int getCurrentFramebufferID();
		static int getUniformName(const unsigned int rendererId, const std::string& name);
		static void setTexture(const int uniformLocation, const int textureUnit, const unsigned int textureType, const unsigned int textureId);
		static void setUniform(const int uniformLocation, const int val);
		static void setUniform(const int uniformLocation, const float val);
		static void setUniform(const int uniformLocation, const float v0, const float v1);
		static void setUniform(const int uniformLocation, const float v0, const float v1, const float v2);
		static void setUniform(const int uniformLocation, const float v0, const float v1, const float v2, const float v3);
		static void setUniformMat3(const int uniformLocation, const float* matrixArrayPointer, int numMatrices);
		static void setUniformMat4(const int uniformLocation, const float* matrixArrayPointer, int numMatrices);
	};

}
