#pragma once
#include "GameComponent.h"

class NoiseTexture :
	public GameComponent
{
public:
	GENINSTANCE(GameComponent);

	NoiseTexture();
	~NoiseTexture();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);

	void generateTexture(float baseFreq, float persistence, bool periodic);
	GLuint storeTexture();
private:
	int m_width = 256;
	int m_height = 256;
	GLubyte* m_textureData = nullptr;
};

