#pragma once
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include "Color.h"
#include "RendererComponent.h"
#include "UI\Spritesheet.h"
#include "ResourceRef.h"
#include "Resources\SpriteSheetResource.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Components
{

	class SpriteRenderer : public RendererComponent, Components::SelfRegister<SpriteRenderer>
	{
	public:
		GENINSTANCE(SpriteRenderer);

		SpriteRenderer();
		~SpriteRenderer();

		void postInit();
		void deserialize(const YAML::Node& compNode);

		Color getColor() const { return m_color; }
		void setColor(Color const& color) {
			if (color == m_color)
			{
				return;
			}

			m_color = color;
			markComponentAsDirty(Rendering::MeshComponents::Colors);
		}

		template<class Archive>
		void save(Archive& archive) const;

		template<class Archive>
		void load(Archive& archive);

	private:
		Color m_color;
		ResourceRef<Resources::SpriteSheetResource> m_spriteSheet;
		ResourceRef<Resources::ShaderResource> m_Shader;
		int m_SpriteIndex;
		UI::SpriteReference* m_sprite;
		glm::vec2 m_size;

		std::vector<glm::vec3> generateVertices();
		std::vector<glm::vec2> generateTexCoords();
		std::vector<Color> generateVertexColors();
		std::vector<unsigned int> generateTriangleIndices();

		unsigned int generateNumVertices()
		{
			switch (m_sprite->type)
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
			switch (m_sprite->type)
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

CEREAL_REGISTER_TYPE(DerydocaEngine::Components::SpriteRenderer);