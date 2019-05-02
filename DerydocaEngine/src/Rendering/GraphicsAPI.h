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
		static void setUniform(const int uniformLocation, const glm::vec3& val);
		static void setUniform(const int uniformLocation, const glm::vec4& val);
		static void setUniform(const int uniformLocation, const glm::mat3& val);
		static void setUniform(const int uniformLocation, const glm::mat4& val);
		static void setUniform(const int uniformLocation, const Color& val);
	};

}
