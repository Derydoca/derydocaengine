#pragma once

#include <string>
#include <GL/glew.h>

namespace DerydocaEngine::Rendering {
	struct TextureParameters;
}

namespace DerydocaEngine::Rendering
{

	class Texture
	{
	public:
		Texture();
		Texture(std::string const& fileName);
		Texture(std::string const& fileName, TextureParameters* const& params);
		Texture(std::string const& xpos, std::string const& xneg, std::string const& ypos, std::string const& yneg, std::string const& zpos, std::string const& zneg);
		~Texture();

		void bind(unsigned int const& unit);
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		GLenum getTextureType() const { return m_textureType; }
		GLuint getTextureId() const { return m_texture; }
		void updateBuffer(GLubyte * const& data, int const& width, int const& height, int const& channels, TextureParameters* const& params);

		static GLenum ChannelsToPixelFormat(int const& numChannels)
		{
			switch (numChannels)
			{
			case 1:
				return GL_RED;
			case 2:
				return GL_RG;
			case 3:
				return GL_RGB;
			default:
				return GL_RGBA;
			}
		}
	protected:
		int m_width, m_height;
		GLuint m_texture;
		GLenum m_textureType;
	private:
		Texture(Texture const& other) {}
		void operator=(Texture const& other) {}
	};

}
