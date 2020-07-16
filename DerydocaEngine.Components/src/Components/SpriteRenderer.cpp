#include "EngineComponentsPch.h"
#include "SpriteRenderer.h"

#include "Rendering\Material.h"
#include "Rendering\Shader.h"
#include "Helpers\YamlTools.h"

namespace DerydocaEngine::Components
{

	SpriteRenderer::SpriteRenderer() :
		m_Color(1.0f, 1.0f, 1.0f, 1.0f),
		m_SpriteSheet(),
		m_Sprite(nullptr),
		m_Size(0.0f, 0.0f)
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::postInit()
	{
		if (m_SpriteSheet)
		{
			auto ss = std::static_pointer_cast<UI::SpriteSheet>(m_SpriteSheet.GetSmartPointer()->getResourceObjectPointer());
			auto spriteSheetTexture = ss->getTexture();
			getMaterial()->setTexture("SpriteSheet", spriteSheetTexture);
			markComponentAsDirty(Rendering::MeshComponents::All);
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

		bool nativeSize = m_Size.x == 0.0f && m_Size.y == 0.0f;
		float c0 = 0.0f;
		float c3 = nativeSize ? m_Sprite->size.x : m_Size.x;
		float r0 = -(0.0f);
		float r3 = -(nativeSize ? m_Sprite->size.y : m_Size.y);

		// Set the corner positions
		vertices.push_back(glm::vec3(c0, r0, 0.0f));
		vertices.push_back(glm::vec3(c3, r0, 0.0f));
		vertices.push_back(glm::vec3(c0, r3, 0.0f));
		vertices.push_back(glm::vec3(c3, r3, 0.0f));

		// If this is a nine-slice sprite, set the positions for the remaining verts
		if (m_Sprite->type == UI::SpriteType::NineSlice)
		{
			int2 spriteSize = m_Sprite->size;
			float4 spriteSlice = m_Sprite->slice;
			float c1 = c0 + (spriteSize.x * spriteSlice.w);
			float c2 = c3 - (spriteSize.x * (1 - spriteSlice.y));
			float r1 = r0 - (spriteSize.y * spriteSlice.x);
			float r2 = r3 + (spriteSize.y * (1 - spriteSlice.z));

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

		float c0 = m_Sprite->texPosition.getX();
		float c3 = m_Sprite->texPosition.getDX();
		float r0 = 1.0f - m_Sprite->texPosition.getY();
		float r3 = 1.0f - m_Sprite->texPosition.getDY();

		// Set the corners
		texCoords.push_back(glm::vec2(c0, r0));
		texCoords.push_back(glm::vec2(c3, r0));
		texCoords.push_back(glm::vec2(c0, r3));
		texCoords.push_back(glm::vec2(c3, r3));

		// If this is a nine-slice sprite, set the UVs for the remaining verts
		if (m_Sprite->type == UI::SpriteType::NineSlice)
		{
			float texw = c3 - c0;
			float texh = r3 - r0;

			float4 spriteSlice = m_Sprite->slice;
			float c1 = c0 + (texw * spriteSlice.w);
			float c2 = c0 + (texw * spriteSlice.y);
			float r1 = r0 + (texh * spriteSlice.x);
			float r2 = r0 + (texh * spriteSlice.z);

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
			vertexColors.push_back(m_Color);
		}

		return vertexColors;
	}

	std::vector<unsigned int> SpriteRenderer::generateTriangleIndices()
	{
		switch (m_Sprite->type)
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

	SERIALIZE_FUNC_SAVE(archive, SpriteRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::RendererComponent),
			SERIALIZE(m_Color),
			SERIALIZE(m_SpriteSheet),
			SERIALIZE(m_SpriteIndex),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Size)
		);
	}

	SERIALIZE_FUNC_LOAD(archive, SpriteRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::RendererComponent),
			SERIALIZE(m_Color),
			SERIALIZE(m_SpriteSheet),
			SERIALIZE(m_SpriteIndex),
			SERIALIZE(m_Shader),
			SERIALIZE(m_Size));

		auto shader = std::static_pointer_cast<Rendering::Shader>(m_Shader->getResourceObjectPointer());
		auto material = std::make_shared<Rendering::Material>();
		material->setShader(shader);
		setMaterial(material);

		auto ss = std::static_pointer_cast<UI::SpriteSheet>(m_SpriteSheet.GetSmartPointer()->getResourceObjectPointer());
		m_Sprite = ss->getSpriteReference(m_SpriteIndex);

		markComponentAsDirty(Rendering::MeshComponents::All);
		updateMesh();
	}
}
