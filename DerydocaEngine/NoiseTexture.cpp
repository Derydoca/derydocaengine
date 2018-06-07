#include "NoiseTexture.h"
#include <glm/gtc/noise.hpp>
#include "MeshRenderer.h"

using namespace glm;

NoiseTexture::NoiseTexture()
{
	m_texture = new Texture();
}

NoiseTexture::~NoiseTexture()
{
	delete m_texture;
}

void NoiseTexture::init()
{
	MeshRenderer* mr = getComponent<MeshRenderer>();
	m_material = mr->getMaterial();

	generateNoiseTexture(m_baseFrequency, m_persistence, m_periodic);
}

void NoiseTexture::deserialize(YAML::Node compNode)
{
	YAML::Node widthNode = compNode["width"];
	if (widthNode)
	{
		m_width = widthNode.as<int>();
	}

	YAML::Node heightNode = compNode["height"];
	if (heightNode)
	{
		m_height = heightNode.as<int>();
	}

	YAML::Node textureNameNode = compNode["textureName"];
	if (textureNameNode)
	{
		m_textureName = textureNameNode.as<string>();
	}

	YAML::Node baseFrequencyNode = compNode["baseFrequency"];
	if (baseFrequencyNode)
	{
		m_baseFrequency = baseFrequencyNode.as<float>();
	}

	YAML::Node persistenceNode = compNode["persistence"];
	if (persistenceNode)
	{
		m_persistence = persistenceNode.as<float>();
	}

	YAML::Node periodicNode = compNode["periodic"];
	if (periodicNode)
	{
		m_periodic = periodicNode.as<bool>();
	}
}

void NoiseTexture::generateNoiseTexture(float baseFreq, float persistence, bool periodic)
{
	GLubyte* textureData = new GLubyte[m_width * m_height * 4];

	double xFactor = 1.0f / (m_width - 1);
	double yFactor = 1.0f / (m_height - 1);

	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			float x = xFactor * col;
			float y = yFactor * row;
			float sum = 0.0f;
			float freq = baseFreq;
			float persist = persistence;
			for (int oct = 0; oct < 4; oct++) {
				glm::vec2 p(x * freq, y * freq);

				float val = 0.0f;
				if (periodic) {
					val = glm::perlin(p, glm::vec2(freq)) * persist;
				}
				else {
					val = glm::perlin(p) * persist;
				}

				sum += val;

				float result = (sum + 1.0f) / 2.0f;

				// Clamp strictly between 0 and 1
				result = result > 1.0f ? 1.0f : result;
				result = result < 0.0f ? 0.0f : result;

				// Store in texture
				textureData[((row * m_width + col) * 4) + oct] = (GLubyte)(result * 255.0f);
				freq *= 2.0f;
				persist *= persistence;
			}
		}
	}

	TextureParameters texParams;
	texParams.setWrapModeS(TextureWrapMode::REPEAT);
	texParams.setWrapModeT(TextureWrapMode::REPEAT);
	m_texture->updateBuffer(textureData, m_width, m_height, GL_RGBA, &texParams);

	m_material->setTexture(m_textureName, m_texture);

	delete [] textureData;
}

GLuint NoiseTexture::uploadTexture()
{
	GLuint texID;
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_width, m_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return texID;
}
