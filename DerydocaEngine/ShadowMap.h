#pragma once
#include "Texture.h"

class Shader;

namespace DerydocaEngine::Rendering
{

	class ShadowMap : public Texture
	{
	public:
		ShadowMap();
		~ShadowMap();

		void bind(unsigned int const& unit);
		void bindAsRenderTexture();
		void intializeTexture(int const& size);
	private:
		GLuint m_shadowFramebuffer;
	};

}
