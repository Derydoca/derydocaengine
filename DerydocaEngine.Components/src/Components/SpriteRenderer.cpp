#include "EngineComponentsPch.h"
#include "SpriteRenderer.h"

#include "Rendering\Material.h"
#include "Rendering\Shader.h"
#include "Helpers\YamlTools.h"

namespace DerydocaEngine::Components
{

	SpriteRenderer::SpriteRenderer() :
		m_color(1.0f, 1.0f, 1.0f, 1.0f),
		m_spriteSheet(),
		m_sprite(nullptr),
		m_size(0.0f, 0.0f)
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::postInit()
	{
		if (m_spriteSheet)
		{
			auto spriteSheetTexture = m_spriteSheet->getTexture();
			getMaterial()->setTexture("SpriteSheet", spriteSheetTexture);
			markComponentAsDirty(Rendering::MeshComponents::All);
		}
	}

	void SpriteRenderer::deserialize(const YAML::Node& compNode)
	{
		m_spriteSheet = getResourcePointer<UI::SpriteSheet>(compNode, "spriteSheet");
		auto shader = getResourcePointer<Rendering::Shader>(compNode, "shader");
		auto material = std::make_shared<Rendering::Material>();
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

	std::vector<glm::vec3> SpriteRenderer::generateVertices()
	{
		// Get the number of verts we will need for this mesh
		int numVerts = generateNumVertices();
		if (!numVerts)
		{
			return std::vector<glm::vec3>();
		}

		std::vector<glm::vec3> vertices;
		vertices.reserve(numVerts);

		bool nativeSize = m_size.x == 0.0f && m_size.y == 0.0f;
		float c0 = 0.0f;
		float c3 = nativeSize ? m_sprite->getWidth() : m_size.x;
		float r0 = -(0.0f);
		float r3 = -(nativeSize ? m_sprite->getHeight() : m_size.y);

		// Set the corner positions
		vertices.push_back(glm::vec3(c0, r0, 0.0f));
		vertices.push_back(glm::vec3(c3, r0, 0.0f));
		vertices.push_back(glm::vec3(c0, r3, 0.0f));
		vertices.push_back(glm::vec3(c3, r3, 0.0f));

		// If this is a nine-slice sprite, set the positions for the remaining verts
		if (m_sprite->getType() == UI::SpriteType::NineSlice)
		{
			float c1 = c0 + (m_sprite->getWidth() * m_sprite->getSliceLeft());
			float c2 = c3 - (m_sprite->getWidth() * (1 - m_sprite->getSliceRight()));
			float r1 = r0 - (m_sprite->getHeight() * m_sprite->getSliceTop());
			float r2 = r3 + (m_sprite->getHeight() * (1 - m_sprite->getSliceBottom()));

			// Fill the gap of two in the first line
			vertices.push_back(glm::vec3(c1, r0, 0.0f));
			vertices.push_back(glm::vec3(c2, r0, 0.0f));

			// Build the second line
			vertices.push_back(glm::vec3(c0, r1, 0.0f));
			vertices.push_back(glm::vec3(c1, r1, 0.0f));
			vertices.push_back(glm::vec3(c2, r1, 0.0f));
			vertices.push_back(glm::vec3(c3, r1, 0.0f));

			// Build the third line
			vertices.push_back(glm::vec3(c0, r2, 0.0f));
			vertices.push_back(glm::vec3(c1, r2, 0.0f));
			vertices.push_back(glm::vec3(c2, r2, 0.0f));
			vertices.push_back(glm::vec3(c3, r2, 0.0f));

			// Fill the gap of the last line
			vertices.push_back(glm::vec3(c1, r3, 0.0f));
			vertices.push_back(glm::vec3(c2, r3, 0.0f));
		}

		return vertices;
	}

	std::vector<glm::vec2> SpriteRenderer::generateTexCoords()
	{
		// Get the number of verts we will need for this mesh
		int numVerts = generateNumVertices();
		if (!numVerts)
		{
			return std::vector<glm::vec2>();
		}

		std::vector<glm::vec2> texCoords;
		texCoords.reserve(numVerts);

		float c0 = m_sprite->getTexPosition().getX();
		float c3 = m_sprite->getTexPosition().getDX();
		float r0 = 1.0f - m_sprite->getTexPosition().getY();
		float r3 = 1.0f - m_sprite->getTexPosition().getDY();

		// Set the corners
		texCoords.push_back(glm::vec2(c0, r0));
		texCoords.push_back(glm::vec2(c3, r0));
		texCoords.push_back(glm::vec2(c0, r3));
		texCoords.push_back(glm::vec2(c3, r3));

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
			texCoords.push_back(glm::vec2(c1, r0));
			texCoords.push_back(glm::vec2(c2, r0));

			// Build the second line
			texCoords.push_back(glm::vec2(c0, r1));
			texCoords.push_back(glm::vec2(c1, r1));
			texCoords.push_back(glm::vec2(c2, r1));
			texCoords.push_back(glm::vec2(c3, r1));

			// Build the third line
			texCoords.push_back(glm::vec2(c0, r2));
			texCoords.push_back(glm::vec2(c1, r2));
			texCoords.push_back(glm::vec2(c2, r2));
			texCoords.push_back(glm::vec2(c3, r2));

			// Fill the gap of the last line
			texCoords.push_back(glm::vec2(c1, r3));
			texCoords.push_back(glm::vec2(c2, r3));
		}

		return texCoords;
	}

	std::vector<Color> SpriteRenderer::generateVertexColors()
	{
		// Get the number of verts we will need for this mesh
		int numVerts = generateNumVertices();
		if (!numVerts)
		{
			return std::vector<Color>();
		}

		std::vector<Color> vertexColors;
		vertexColors.reserve(numVerts);

		for (int i = 0; i < numVerts; i++)
		{
			vertexColors.push_back(m_color);
		}

		return vertexColors;
	}

	std::vector<unsigned int> SpriteRenderer::generateTriangleIndices()
	{
		switch (m_sprite->getType())
		{
		case UI::SpriteType::Sprite:
			return {
				0, 2, 1,
				1, 2, 3
			};
		case UI::SpriteType::NineSlice:
			return {
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
			return std::vector<unsigned int>();
		}
	}

}
