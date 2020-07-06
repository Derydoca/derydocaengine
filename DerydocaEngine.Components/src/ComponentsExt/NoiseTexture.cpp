#include "EngineComponentsPch.h"
#include "NoiseTexture.h"

#include <GL\glew.h>
#include <glm/gtc/noise.hpp>
#include "Components\Camera.h"
#include "Components\MeshRenderer.h"
#include "Rendering\Texture.h"
#include "Rendering\TextureParameters.h"

namespace DerydocaEngine::Ext
{

	NoiseTexture::NoiseTexture()
	{
		m_Texture = std::make_shared<Rendering::Texture>();
	}

	NoiseTexture::~NoiseTexture()
	{
	}

	void NoiseTexture::init()
	{
		auto mr = getComponentInChildren<Components::MeshRenderer>();
		if (mr)
		{
			m_Material = mr->getMaterial();
		}
		else
		{
			auto cam = getComponentInChildren<Components::Camera>();
			if (cam)
			{
				m_Material = cam->getPostProcessMaterial();
			}
		}

		generateNoiseTexture();
	}

	void NoiseTexture::deserialize(const YAML::Node& compNode)
	{
		YAML::Node widthNode = compNode["width"];
		if (widthNode)
		{
			m_Size.x = widthNode.as<int>();
		}

		YAML::Node heightNode = compNode["height"];
		if (heightNode)
		{
			m_Size.y = heightNode.as<int>();
		}

		YAML::Node textureNameNode = compNode["textureName"];
		if (textureNameNode)
		{
			m_TextureName = textureNameNode.as<std::string>();
		}

		YAML::Node baseFrequencyNode = compNode["baseFrequency"];
		if (baseFrequencyNode)
		{
			m_BaseFrequency = baseFrequencyNode.as<float>();
		}

		YAML::Node persistenceNode = compNode["persistence"];
		if (persistenceNode)
		{
			m_Persistence = persistenceNode.as<float>();
		}

		YAML::Node periodicNode = compNode["periodic"];
		if (periodicNode)
		{
			m_Periodic = periodicNode.as<bool>();
		}

		YAML::Node seamlessNode = compNode["seamless"];
		if (seamlessNode)
		{
			m_Seamless = seamlessNode.as<bool>();
		}
	}

	void NoiseTexture::generateNoiseTexture()
	{
		GLubyte* textureData = new GLubyte[m_Size.x * m_Size.y * 4];

		float xRange = 1.0f;
		float yRange = 1.0f;
		float xFactor = xRange / (m_Size.x - 1);
		float yFactor = yRange / (m_Size.y - 1);

		for (int row = 0; row < m_Size.y; row++)
		{
			for (int col = 0; col < m_Size.x; col++)
			{
				float x = xFactor * col;
				float y = yFactor * row;
				float sum = 0.0f;
				float freq = m_BaseFrequency;
				float persist = m_Persistence;
				for (int oct = 0; oct < 4; oct++) {

					float val = 0.0f;
					if (m_Seamless)
					{
						float a, b, c, d;

						glm::vec2 aPos = glm::vec2(x, y) * freq;
						glm::vec2 bPos = glm::vec2(x + xRange, y) * freq;
						glm::vec2 cPos = glm::vec2(x, y + yRange) * freq;
						glm::vec2 dPos = glm::vec2(x + xRange, y + yRange) * freq;

						if (m_Periodic)
						{
							a = perlin(aPos);
							b = perlin(bPos);
							c = perlin(cPos);
							d = perlin(dPos);
						}
						else
						{
							a = perlin(aPos, glm::vec2(freq));
							b = perlin(bPos, glm::vec2(freq));
							c = perlin(cPos, glm::vec2(freq));
							d = perlin(dPos, glm::vec2(freq));
						}

						a *= persist;
						b *= persist;
						c *= persist;
						d *= persist;

						float xmix = 1.0f - x / xRange;
						float ymix = 1.0f - y / yRange;
						float x1 = glm::mix(a, b, xmix);
						float x2 = glm::mix(c, d, xmix);

						val = glm::mix(x1, x2, ymix);
					}
					else
					{
						glm::vec2 p(x * freq, y * freq);
						if (m_Periodic)
						{
							val = glm::perlin(p, glm::vec2(freq));
						}
						else
						{
							val = glm::perlin(p);
						}

						val *= persist;
					}

					sum += val;

					float result = (sum + 1.0f) / 2.0f;

					// Clamp strictly between 0 and 1
					result = result > 1.0f ? 1.0f : result;
					result = result < 0.0f ? 0.0f : result;

					// Store in texture
					textureData[((row * m_Size.x + col) * 4) + oct] = (GLubyte)(result * 255.0f);
					freq *= 2.0f;
					persist *= m_Persistence;
				}
			}
		}

		Rendering::TextureParameters texParams;
		texParams.setWrapModeS(Rendering::TextureWrapMode::REPEAT);
		texParams.setWrapModeT(Rendering::TextureWrapMode::REPEAT);
		m_Texture->updateBuffer(textureData, m_Size.x, m_Size.y, 4, &texParams);

		if (m_Material)
		{
			m_Material->setTexture(m_TextureName, m_Texture);
		}

		delete[] textureData;
	}

}
