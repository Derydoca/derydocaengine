#pragma once
#include "Rendering\Texture.h"
#include "Rendering\RenderingMode.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Rendering
{

	class RenderTexture : public Texture
	{
	public:
		RenderTexture();
		RenderTexture(RenderingMode renderingMode);
		~RenderTexture();

		void bind(unsigned int const& unit);
		void bindDeferredTextures(std::shared_ptr<Shader> shader); // TODO: This is a hack. Find another way to bind the deferred textures
		void bindAsRenderTexture();
		float getAspectRatio();
		void initializeTexture(int const& width, int const& height);

		RenderingMode getRenderingMode() { return m_renderingMode; }

	private:
		void createRenderbuffer(int textureUnit, GLenum format, unsigned int & textureId);
		void createRenderbuffers_Deferred();
		void destroyGraphicsApiObjects();

	private:
		RenderingMode m_renderingMode;
		unsigned int m_framebuffer;
		std::vector<unsigned int> m_renderBufferIds;

	};

}