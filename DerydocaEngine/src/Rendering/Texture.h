#pragma once

#include <string>

namespace DerydocaEngine::Rendering {
	struct TextureParameters;
}

namespace DerydocaEngine::Rendering
{

	class Texture
	{
	public:
		Texture();
		Texture(std::string const& fileName, TextureParameters* const& params = nullptr);
		Texture(std::string const& xpos, std::string const& xneg, std::string const& ypos, std::string const& yneg, std::string const& zpos, std::string const& zneg);
		~Texture();

		void bind(unsigned int const& unit) const;
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		unsigned int getTextureType() const { return m_textureType; }
		unsigned int getRendererId() const { return m_rendererId; }
		void updateBuffer(unsigned char * const& data, int const& width, int const& height, int const& channels, TextureParameters* const& params);

		unsigned int channelsToPixelFormat(int const& numChannels) const;
	protected:
		unsigned int m_rendererId;
		int m_width;
		int m_height;
		unsigned int m_textureType;
	private:
		Texture(Texture const& other) {}
		void operator=(Texture const& other) {};

		void deleteTexture();
	};

}
