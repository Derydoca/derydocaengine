#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Color.h"
#include <glm\glm.hpp>

class RendererComponent : public GameComponent
{
public:
	Material* getMaterial() const { return m_material; }
	void setMaterial(Material* material) { m_material = material; }
protected:
	void updateMesh();
	void markComponentAsDirty(MeshComponents component)
	{
		m_dirtyComponents = (MeshComponents)(m_dirtyComponents | component);
	}

	void render(MatrixStack* matrixStack);
	void renderMesh(MatrixStack* matrixStack, Material* material, Projection projection, Transform* projectionTransform);

	virtual vec3* generateVertices() = 0;
	virtual vec2* generateTexCoords() { return nullptr; }
	virtual Color* generateVertexColors() { return nullptr; }
	virtual unsigned int* generateTriangleIndices() = 0;
	virtual vec3* generateTangents() { return nullptr; }
	virtual vec3* generateBitangents() { return nullptr; }
	virtual vec3* generateNormals() { return nullptr; }
	virtual unsigned int getNumVertices() { return 0; }
	virtual unsigned int getNumIndices() { return 0; }

	Mesh * m_mesh;
	Material * m_material;
	MeshComponents m_dirtyComponents;
	vec3* m_vertices = nullptr;
	vec3* m_tangents = nullptr;
	vec3* m_bitangents = nullptr;
	vec3* m_normals = nullptr;
	vec2* m_texCoords = nullptr;
	Color* m_vertexColors = nullptr;
	unsigned int* m_triangleIndices = nullptr;
	unsigned int m_numVertices = 0;
	unsigned int m_numIndices = 0;
};
