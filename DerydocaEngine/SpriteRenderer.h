#pragma once
#include <glm/glm.hpp>
#include "GameComponent.h"
#include "MeshRenderer.h"
#include "Color.h"
#include "SpriteSheet.h"

using namespace glm;

class SpriteRenderer : public GameComponent
{
public:
	GENINSTANCE(SpriteRenderer);

	SpriteRenderer();
	~SpriteRenderer();

	virtual void init();
	virtual void postInit();
	virtual void deserialize(YAML::Node compNode);
private:
	MeshRenderer * m_meshRenderer;
	Color m_color;
	SpriteSheet* m_spriteSheet;
	Material* m_material;
	Mesh m_mesh;
	vec3* m_meshVerts;
	vec2* m_meshUvs;
	Color* m_meshColors;
	unsigned int* m_meshIndices;
	MeshComponents m_dirtyMeshComponents;
	SpriteReference* m_sprite;
	vec2 m_size;
	int m_numVerts;

	void updateMesh();
	void updateMeshVerts();
	void updateMeshUVs();
	void updateMeshColors();
	void updateMeshIndices();

	int getNumVertsFromSpriteType() {
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
	unsigned int getNumIndicesFromSpriteType()
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

