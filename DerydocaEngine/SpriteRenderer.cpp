#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::init()
{
	m_meshRenderer = getComponent<MeshRenderer>();
}

void SpriteRenderer::postInit()
{
	if (m_spriteSheet && m_meshRenderer)
	{
		Texture* spriteSheetTexture = m_spriteSheet->getTexture();
		m_meshRenderer->setMaterial(m_material);
		m_meshRenderer->getMaterial()->setTexture("SpriteSheet", spriteSheetTexture);
		m_dirtyMeshComponents = MeshComponents::All;
	}

	updateMesh();
}

void SpriteRenderer::deserialize(YAML::Node compNode)
{
	m_spriteSheet = loadResource<SpriteSheet*>(compNode, "spriteSheet");
	Shader* shader = loadResource<Shader*>(compNode, "shader");
	m_material = new Material();
	m_material->setShader(shader);

	YAML::Node sizeNode = compNode["size"];
	if (sizeNode)
	{
		m_size = sizeNode.as<vec2>();
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
void SpriteRenderer::updateMesh()
{
	if(m_dirtyMeshComponents & MeshComponents::TexCoords)
	{
		updateMeshUVs();
	}
	if (m_dirtyMeshComponents & MeshComponents::Colors)
	{
		updateMeshColors();
	}
	if (m_dirtyMeshComponents & MeshComponents::Positions)
	{
		updateMeshVerts();
	}
	if (m_dirtyMeshComponents & MeshComponents::Indices)
	{
		updateMeshIndices();
	}

	// Store the number of vertices in this mesh
	m_numVerts = getNumVertsFromSpriteType();

	// Load the mesh with the updated buffers
	m_mesh.loadMeshComponentData(m_dirtyMeshComponents, m_numVerts, m_meshVerts, nullptr, nullptr, m_meshUvs, nullptr, getNumIndicesFromSpriteType(), m_meshIndices, m_meshColors);

	// Mark all components as clean
	m_dirtyMeshComponents = MeshComponents::None;

	m_meshRenderer->setMesh(&m_mesh);
}

void SpriteRenderer::updateMeshVerts()
{
	// Get the number of verts we will need for this mesh
	int numVerts = getNumVertsFromSpriteType();
	if (!numVerts)
	{
		return;
	}

	if (m_numVerts != numVerts)
	{
		delete[] m_meshVerts;
		m_meshVerts = new vec3[numVerts];
	}

	//vec2 b = m_size.x == 0.0f && m_size.y == 0.0f ? vec2(m_sprite->getWidth(), m_sprite->getHeight()) : m_size;
	bool nativeSize = m_size.x == 0.0f && m_size.y == 0.0f;
	float c0 = 0.0f;
	float c3 = nativeSize ? m_sprite->getWidth() : m_size.x;
	float r0 = -(0.0f);
	float r3 = -(nativeSize ? m_sprite->getHeight() : m_size.y);

	// Set the corner positions
	m_meshVerts[0] = vec3(c0, r0, 0.0f);
	m_meshVerts[1] = vec3(c3, r0, 0.0f);
	m_meshVerts[2] = vec3(c0, r3, 0.0f);
	m_meshVerts[3] = vec3(c3, r3, 0.0f);

	// If this is a nine-slice sprite, set the positions for the remaining verts
	if (m_sprite->getType() == SpriteType::NineSlice)
	{
		float c1 = c0 + (m_sprite->getWidth() * m_sprite->getSliceLeft());
		float c2 = c3 - (m_sprite->getWidth() * (1 - m_sprite->getSliceRight()));
		float r1 = r0 - (m_sprite->getHeight() * m_sprite->getSliceTop());
		float r2 = r3 + (m_sprite->getHeight() * (1 - m_sprite->getSliceBottom()));

		// Fill the gap of two in the first line
		m_meshVerts[4] = vec3(c1, r0, 0.0f);
		m_meshVerts[5] = vec3(c2, r0, 0.0f);

		// Build the second line
		m_meshVerts[6] = vec3(c0, r1, 0.0f);
		m_meshVerts[7] = vec3(c1, r1, 0.0f);
		m_meshVerts[8] = vec3(c2, r1, 0.0f);
		m_meshVerts[9] = vec3(c3, r1, 0.0f);

		// Build the third line
		m_meshVerts[10] = vec3(c0, r2, 0.0f);
		m_meshVerts[11] = vec3(c1, r2, 0.0f);
		m_meshVerts[12] = vec3(c2, r2, 0.0f);
		m_meshVerts[13] = vec3(c3, r2, 0.0f);

		// Fill the gap of the last line
		m_meshVerts[14] = vec3(c1, r3, 0.0f);
		m_meshVerts[15] = vec3(c2, r3, 0.0f);
	}
}

void SpriteRenderer::updateMeshUVs()
{
	// Get the number of verts we will need for this mesh
	int numVerts = getNumVertsFromSpriteType();
	if (!numVerts)
	{
		return;
	}

	if (m_numVerts != numVerts)
	{
		delete[] m_meshUvs;
		m_meshUvs = new vec2[numVerts];
	}

	float c0 = m_sprite->getTexPosition().getX();
	float c3 = m_sprite->getTexPosition().getDX();
	float r0 = 1.0f - m_sprite->getTexPosition().getY();
	float r3 = 1.0f - m_sprite->getTexPosition().getDY();

	// Set the corners
	m_meshUvs[0] = vec2(c0, r0);
	m_meshUvs[1] = vec2(c3, r0);
	m_meshUvs[2] = vec2(c0, r3);
	m_meshUvs[3] = vec2(c3, r3);

	// If this is a nine-slice sprite, set the UVs for the remaining verts
	if (m_sprite->getType() == SpriteType::NineSlice)
	{

		float texw = c3 - c0;
		float texh = r3 - r0;

		float c1 = c0 + (texw * m_sprite->getSliceLeft());
		float c2 = c0 + (texw * m_sprite->getSliceRight());
		float r1 = r0 + (texh * m_sprite->getSliceTop());
		float r2 = r0 + (texh * m_sprite->getSliceBottom());
		
		// Fill the gap of two in the first line
		m_meshUvs[4] = vec2(c1, r0);
		m_meshUvs[5] = vec2(c2, r0);
		
		// Build the second line
		m_meshUvs[6] = vec2(c0, r1);
		m_meshUvs[7] = vec2(c1, r1);
		m_meshUvs[8] = vec2(c2, r1);
		m_meshUvs[9] = vec2(c3, r1);
		
		// Build the third line
		m_meshUvs[10] = vec2(c0, r2);
		m_meshUvs[11] = vec2(c1, r2);
		m_meshUvs[12] = vec2(c2, r2);
		m_meshUvs[13] = vec2(c3, r2);
		
		// Fill the gap of the last line
		m_meshUvs[14] = vec2(c1, r3);
		m_meshUvs[15] = vec2(c2, r3);
	}
}

void SpriteRenderer::updateMeshColors()
{
	// Get the number of verts we will need for this mesh
	int numVerts = getNumVertsFromSpriteType();
	if (!numVerts)
	{
		return;
	}

	if (m_numVerts != numVerts)
	{
		delete[] m_meshColors;
		m_meshColors = new Color[numVerts];
	}

	for (int i = 0; i < numVerts; i++)
	{
		m_meshColors[i] = m_color;
	}
}

void SpriteRenderer::updateMeshIndices()
{
	// Get the number of verts we will need for this mesh
	int numVerts = getNumVertsFromSpriteType();
	if (!numVerts)
	{
		return;
	}

	if (m_numVerts != numVerts)
	{
		delete[] m_meshIndices;

		switch (m_sprite->getType())
		{
		case SpriteType::Sprite:
			m_meshIndices = new unsigned int[2 * 3] {
					0, 2, 1,
					1, 2, 3
			};
			break;
		case SpriteType::NineSlice:
			m_meshIndices = new unsigned int[18 * 3] {
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
			break;
		default:
			break;
		}
	}

}
