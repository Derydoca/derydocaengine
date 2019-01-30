#pragma once
#include "Rendering\Texture.h"

namespace DerydocaEngine::Rendering
{

	class RenderTexture : public Texture
	{
	public:
		RenderTexture();
		RenderTexture(int const& width, int const& height);
		~RenderTexture();

		void bind(unsigned int const& unit);
		void bindAsRenderTexture();
		float getAspectRatio();
		void initializeTexture(int const& width, int const& height);
	private:
		void deleteVbo();
	private:
		unsigned int m_framebuffer;
		unsigned int m_depthbuffer;
	};

}