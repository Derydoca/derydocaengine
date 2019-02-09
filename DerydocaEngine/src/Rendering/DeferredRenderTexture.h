#pragma once
#include "Rendering\Texture.h"

namespace DerydocaEngine::Rendering
{

	class DeferredRenderTexture : public Texture
	{
	public:
		DeferredRenderTexture();
		~DeferredRenderTexture();

		void bindAsRenderTexture();
		void initializeBuffer(int width, int height);
	private:
		void createRenderbuffer(int textureUnit, GLenum format, unsigned int & textureId);
		void deleteBuffers();
	private:
		unsigned int m_framebuffer;

		// Renderbuffers
		unsigned int m_depthBuffer;
		unsigned int m_positionBuffer;
		unsigned int m_normalBuffer;
		unsigned int m_colorBuffer;
	};

}
