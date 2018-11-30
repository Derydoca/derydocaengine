#pragma once
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include "Color.h"
#include "RendererComponent.h"
#include "UI\Spritesheet.h"

namespace DerydocaEngine::Components
{

	class SpriteRenderer : public RendererComponent, Components::SelfRegister<SpriteRenderer>
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
			markComponentAsDirty(Rendering::MeshComponents::Colors);
		}
	private:
		Color m_color;
		std::shared_ptr<UI::SpriteSheet> m_spriteSheet;
		UI::SpriteReference* m_sprite;
		glm::vec2 m_size;

		std::vector<glm::vec3> generateVertices();
		std::vector<glm::vec2> generateTexCoords();
		std::vector<Color> generateVertexColors();
		std::vector<unsigned int> generateTriangleIndices();

		unsigned int generateNumVertices()
		{
			switch (m_sprite->getType())
			{
			case UI::SpriteType::Sprite:
				return 4;
			case UI::SpriteType::NineSlice:
				return 16;
			default:
				return 0;
			}
		}

		unsigned int generateNumIndices()
		{
			switch (m_sprite->getType())
			{
			case UI::SpriteType::Sprite:
				return 2 * 3;
			case UI::SpriteType::NineSlice:
				return 18 * 3;
			default:
				return 0;
			}
		}

	};

}
