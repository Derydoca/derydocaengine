#pragma once
#include "GameComponent.h"
#include "Material.h"

using namespace std;

class NoiseTexture : public GameComponent
{
public:
	GENINSTANCE(NoiseTexture);

	NoiseTexture();
	~NoiseTexture();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);

	void generateNoiseTexture(float baseFreq, float persistence, bool periodic);
private:
	int m_width = 256;
	int m_height = 256;
	string m_textureName = "NoiseTexture";
	Material* m_material = nullptr;
	Texture* m_texture = nullptr;
	float m_baseFrequency = 4.0;
	float m_persistence = 1.0;
	bool m_periodic = false;

	GLuint uploadTexture();
};

