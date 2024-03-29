#pragma once

#include <string>
#include "GraphicsAPI.h"

namespace DerydocaEngine::Rendering {
	struct TextureParameters;
}

namespace DerydocaEngine::Rendering
{
	class Texture
	{
	public:
		Texture();
		Texture(const std::string& fileName);
		Texture(
			const std::string& xpos,
			const std::string& xneg,
			const std::string& ypos,
			const std::string& yneg,
			const std::string& zpos,
			const std::string& zneg
		);
		~Texture();

		void bind(const unsigned int unit) const;
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		TextureType getTextureType() const { return m_textureType; }
		unsigned int getRendererId() const { return m_rendererId; }
		void updateBuffer(
			unsigned char * data,
			const int width,
			const int height,
			const int channels,
			const TextureParameters* params
		);

		InternalTextureFormat channelsToPixelFormat(const int numChannels) const;
	protected:
		unsigned int m_rendererId;
		int m_width;
		int m_height;
		TextureType m_textureType;
	private:
		Texture(Texture const& other) {}
		void operator=(Texture const& other) {};

		void deleteTexture();
	};

}
