#include "NoiseTexture.h"
#include <glm/gtc/noise.hpp>

using namespace glm;

NoiseTexture::NoiseTexture()
{
}

NoiseTexture::~NoiseTexture()
{
}

void NoiseTexture::init()
{
}

void NoiseTexture::deserialize(YAML::Node compNode)
{
}

void NoiseTexture::generateTexture(float baseFreq, float persistence, bool periodic)
{
	//noise::module::Perlin perlinNoise;

	//perlinNoise.SetFrequency(4.0);
	delete[] m_textureData;
	m_textureData = new GLubyte[m_width * m_height * 4];
	double xRange = 1.0;
	double yRange = 1.0;
	double xFactor = xRange / m_width;
	double yFactor = yRange / m_height;

	//perlinNoise.SetOctaveCount(oct + 1);
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			float x = xFactor * col;
			float y = yFactor * row;
			float sum = 0.0f;
			float freq = baseFreq;
			float persist = persistence;

			for (int oct = 0; oct < 4; oct++)
			{
				vec2 p(x * freq, y * freq);

				float val = 0.0f;
				if (periodic)
				{
					val = perlin(p, glm::vec2(freq)) * persist;
				}
				else
				{
					val = perlin(p) * persist;
				}

				sum += val;

				float result = (sum + 1.0f) / 2.0f;

				// Clamp strictly between 0 and 1
				result = result > 1.0f ? 1.0f : result;
				result = result < 0.0f ? 0.0f : result;

				// Store in texture
				m_textureData[((row * m_width + col) * 4) + oct] = (GLubyte)(result * 255.0f);
				freq += 2.0f;
				persist += persistence;
			}
		}
	}


}

GLuint NoiseTexture::storeTexture()
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
