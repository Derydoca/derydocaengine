#pragma once
#include <glm/glm.hpp>
#include "RendererComponent.h"
#include "MeshRenderer.h"
#include "Color.h"
#include "SpriteSheet.h"
#include "Shader.h"

class SpriteRenderer : public RendererComponent
{
public:
	GENINSTANCE(SpriteRenderer);

	void postInit();
	void deserialize(YAML::Node const& compNode);

	Color getColor() const { return m_color; }
	void setColor(Color const& color) {
		if (color == m_color)
		{
			return;
		}

		m_color = color;
		markComponentAsDirty(MeshComponents::Colors);
	}
private:
	Color m_color;
	SpriteSheet* m_spriteSheet;
	SpriteReference* m_sprite;
	glm::vec2 m_size;

	glm::vec3* generateVertices();
	glm::vec2* generateTexCoords();
	Color* generateVertexColors();
	unsigned int* generateTriangleIndices();

	unsigned int generateNumVertices()
	{
		switch (m_sprite->getType())
		{
		case SpriteType::Sprite:
			return 4;
		case SpriteType::NineSlice:
			return 16;
		default:
			return 0;
		}
	}

	unsigned int generateNumIndices()
	{
		switch (m_sprite->getType())
		{
		case SpriteType::Sprite:
			return 2 * 3;
		case SpriteType::NineSlice:
			return 18 * 3;
		default:
			return 0;
		}
	}

};

