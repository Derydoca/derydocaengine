#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);
	Texture(const std::string& xpos, const std::string& xneg, const std::string& ypos, const std::string& yneg, const std::string& zpos, const std::string& zneg);
	~Texture();

	void bind(unsigned int unit);
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	GLenum getTextureType() const { return m_textureType; }
	GLuint getTextureId() const { return m_texture; }
protected:
	int m_width, m_height;
	GLuint m_texture;
	GLenum m_textureType;
private:
	Texture(const Texture& other) {}
	void operator=(const Texture& other) {}
};

