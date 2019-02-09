#pragma once
#include "Rendering\Texture.h"
#include "Rendering\RenderingMode.h"

namespace DerydocaEngine::Rendering
{

	class RenderTexture : public Texture
	{
	public:
		RenderTexture();
		RenderTexture(RenderingMode renderingMode);
		~RenderTexture();

		void bind(unsigned int const& unit);
		void bindAsRenderTexture();
		float getAspectRatio();
		void initializeTexture(int const& width, int const& height);
	private:
		void deleteVbo();
	private:
		RenderingMode m_renderingMode;
		unsigned int m_framebuffer;
		unsigned int m_depthbuffer;
	};

}