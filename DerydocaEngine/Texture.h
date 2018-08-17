#pragma once

#include <string>
#include <GL/glew.h>
#include "TextureParameters.h"

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);
	Texture(const std::string& fileName, TextureParameters* params);
	Texture(const std::string& xpos, const std::string& xneg, const std::string& ypos, const std::string& yneg, const std::string& zpos, const std::string& zneg);
	~Texture();

	void bind(unsigned int unit);
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	GLenum getTextureType() const { return m_textureType; }
	GLuint getTextureId() const { return m_texture; }
	void updateBuffer(GLubyte * data, int width, int height, int channels, TextureParameters* params);

	static GLenum ChannelsToPixelFormat(int numChannels)
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
	Texture(const Texture& other) {}
	void operator=(const Texture& other) {}
};

