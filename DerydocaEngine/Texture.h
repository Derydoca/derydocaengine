#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName);
	Texture(const std::string& xpos, const std::string& xneg, const std::string& ypos, const std::string& yneg, const std::string& zpos, const std::string& zneg);
	~Texture();

	void bind(unsigned int unit);
private:
	Texture(const Texture& other) {}
	void operator=(const Texture& other) {}

	GLuint m_texture;
	GLenum m_textureType;
};

