#include "SpriteRenderer.h"

#include "Material.h"
#include "ObjectLibrary.h"
#include "YamlTools.h"

namespace DerydocaEngine::Components
{

	void SpriteRenderer::postInit()
	{
		if (m_spriteSheet)
		{
			Rendering::Texture* spriteSheetTexture = m_spriteSheet->getTexture();
			getMaterial()->setTexture("SpriteSheet", spriteSheetTexture);
			markComponentAsDirty(Rendering::MeshComponents::All);
		}
	}

	void SpriteRenderer::deserialize(YAML::Node const& compNode)
	{
		m_spriteSheet = getResourceObject<UI::SpriteSheet>(compNode, "spriteSheet");
		Rendering::Shader* shader = getResourceObject<Rendering::Shader>(compNode, "shader");
		Rendering::Material* material = new Rendering::Material();
		material->setShader(shader);
		setMaterial(material);

		YAML::Node sizeNode = compNode["size"];
		if (sizeNode)
		{
			m_size = sizeNode.as<glm::vec2>();
		}

		YAML::Node colorNode = compNode["color"];
		if (colorNode)
		{
			m_color = colorNode.as<Color>();
		}

		YAML::Node spriteIdNode = compNode["spriteId"];
		if (spriteIdNode)
		{
			unsigned int spriteId = spriteIdNode.as<unsigned int>();
			m_sprite = m_spriteSheet->getSpriteReference(spriteId);
		}
	}

	glm::vec3 * SpriteRenderer::generateVertices()
	{
		// Get the number of verts we will need for this mesh
		int numVerts = generateNumVertices();
		if (!numVerts)
		{
			return nullptr;
		}

		glm::vec3* vertices = new glm::vec3[numVerts];

		bool nativeSize = m_size.x == 0.0f && m_size.y == 0.0f;
		float c0 = 0.0f;
		float c3 = nativeSize ? m_sprite->getWidth() : m_size.x;
		float r0 = -(0.0f);
		float r3 = -(nativeSize ? m_sprite->getHeight() : m_size.y);

		// Set the corner positions
		vertices[0] = glm::vec3(c0, r0, 0.0f);
		vertices[1] = glm::vec3(c3, r0, 0.0f);
		vertices[2] = glm::vec3(c0, r3, 0.0f);
		vertices[3] = glm::vec3(c3, r3, 0.0f);

		// If this is a nine-slice sprite, set the positions for the remaining verts
		if (m_sprite->getType() == UI::SpriteType::NineSlice)
		{
			float c1 = c0 + (m_sprite->getWidth() * m_sprite->getSliceLeft());
			float c2 = c3 - (m_sprite->getWidth() * (1 - m_sprite->getSliceRight()));
			float r1 = r0 - (m_sprite->getHeight() * m_sprite->getSliceTop());
			float r2 = r3 + (m_sprite->getHeight() * (1 - m_sprite->getSliceBottom()));

			// Fill the gap of two in the first line
			vertices[4] = glm::vec3(c1, r0, 0.0f);
			vertices[5] = glm::vec3(c2, r0, 0.0f);

			// Build the second line
			vertices[6] = glm::vec3(c0, r1, 0.0f);
			vertices[7] = glm::vec3(c1, r1, 0.0f);
			vertices[8] = glm::vec3(c2, r1, 0.0f);
			vertices[9] = glm::vec3(c3, r1, 0.0f);

			// Build the third line
			vertices[10] = glm::vec3(c0, r2, 0.0f);
			vertices[11] = glm::vec3(c1, r2, 0.0f);
			vertices[12] = glm::vec3(c2, r2, 0.0f);
			vertices[13] = glm::vec3(c3, r2, 0.0f);

			// Fill the gap of the last line
			vertices[14] = glm::vec3(c1, r3, 0.0f);
			vertices[15] = glm::vec3(c2, r3, 0.0f);
		}

		return vertices;
	}

	glm::vec2 * SpriteRenderer::generateTexCoords()
	{
		// Get the number of verts we will need for this mesh
		int numVerts = generateNumVertices();
		if (!numVerts)
		{
			return nullptr;
		}

		glm::vec2* texCoords = new glm::vec2[numVerts];

		float c0 = m_sprite->getTexPosition().getX();
		float c3 = m_sprite->getTexPosition().getDX();
		float r0 = 1.0f - m_sprite->getTexPosition().getY();
		float r3 = 1.0f - m_sprite->getTexPosition().getDY();

		// Set the corners
		texCoords[0] = glm::vec2(c0, r0);
		texCoords[1] = glm::vec2(c3, r0);
		texCoords[2] = glm::vec2(c0, r3);
		texCoords[3] = glm::vec2(c3, r3);

		// If this is a nine-slice sprite, set the UVs for the remaining verts
		if (m_sprite->getType() == UI::SpriteType::NineSlice)
		{
			float texw = c3 - c0;
			float texh = r3 - r0;

			float c1 = c0 + (texw * m_sprite->getSliceLeft());
			float c2 = c0 + (texw * m_sprite->getSliceRight());
			float r1 = r0 + (texh * m_sprite->getSliceTop());
			float r2 = r0 + (texh * m_sprite->getSliceBottom());

			// Fill the gap of two in the first line
			texCoords[4] = glm::vec2(c1, r0);
			texCoords[5] = glm::vec2(c2, r0);

			// Build the second line
			texCoords[6] = glm::vec2(c0, r1);
			texCoords[7] = glm::vec2(c1, r1);
			texCoords[8] = glm::vec2(c2, r1);
			texCoords[9] = glm::vec2(c3, r1);

			// Build the third line
			texCoords[10] = glm::vec2(c0, r2);
			texCoords[11] = glm::vec2(c1, r2);
			texCoords[12] = glm::vec2(c2, r2);
			texCoords[13] = glm::vec2(c3, r2);

			// Fill the gap of the last line
			texCoords[14] = glm::vec2(c1, r3);
			texCoords[15] = glm::vec2(c2, r3);
		}

		return texCoords;
	}

	Color * SpriteRenderer::generateVertexColors()
	{
		// Get the number of verts we will need for this mesh
		int numVerts = generateNumVertices();
		if (!numVerts)
		{
			return nullptr;
		}

		Color* vertexColors = new Color[numVerts];

		for (int i = 0; i < numVerts; i++)
		{
			vertexColors[i] = m_color;
		}

		return vertexColors;
	}

	unsigned int * SpriteRenderer::generateTriangleIndices()
	{
		switch (m_sprite->getType())
		{
		case UI::SpriteType::Sprite:
			return new unsigned int[2 * 3]{
				0, 2, 1,
				1, 2, 3
			};
		case UI::SpriteType::NineSlice:
			return new unsigned int[18 * 3]{
				0, 6, 4, // Row 1 triangles
				4, 6, 7,
				4, 7, 5,
				5, 7, 8,
				5, 8, 1,
				1, 8, 9,

				6, 10, 7, // Row 2 triangles
				7, 10, 11,
				7, 11, 8,
				8, 11, 12,
				8, 12, 9,
				9, 12, 13,

				10, 2, 11, // Row 3 triangles
				11, 2, 14,
				11, 14, 12,
				12, 14, 15,
				12, 15, 13,
				13, 15, 3
			};
		default:
			return nullptr;
		}
	}

}
