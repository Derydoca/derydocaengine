#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Color.h"
#include <glm\glm.hpp>

namespace DerydocaEngine::Components
{

	class RendererComponent : public GameComponent
	{
	public:
		virtual ~RendererComponent() {}
		Rendering::Material* getMaterial() const { return m_material; }
		void setMaterial(Rendering::Material* const& material) { m_material = material; }
	protected:
		void updateMesh();
		void markComponentAsDirty(Rendering::MeshComponents const& component)
		{
			m_dirtyComponents = (Rendering::MeshComponents)(m_dirtyComponents | component);
		}

		void render(Rendering::MatrixStack* const& matrixStack);
		void renderMesh(Rendering::MatrixStack* const& matrixStack, Rendering::Material* const& material, Projection const& projection, Transform* const& projectionTransform);
		unsigned int getNumVertices() const { return m_numVertices; }
		unsigned int getNumIndices() const { return m_numIndices; }

		virtual glm::vec3* generateVertices() = 0;
		virtual glm::vec2* generateTexCoords() { return nullptr; }
		virtual Color* generateVertexColors() { return nullptr; }
		virtual unsigned int* generateTriangleIndices() = 0;
		virtual glm::vec3* generateTangents() { return nullptr; }
		virtual glm::vec3* generateBitangents() { return nullptr; }
		virtual glm::vec3* generateNormals() { return nullptr; }
		virtual unsigned int generateNumVertices() { return 0; }
		virtual unsigned int generateNumIndices() { return 0; }

	private:
		Rendering::Mesh * m_mesh;
		Rendering::Material * m_material;
		Rendering::MeshComponents m_dirtyComponents;
		glm::vec3* m_vertices = nullptr;
		glm::vec3* m_tangents = nullptr;
		glm::vec3* m_bitangents = nullptr;
		glm::vec3* m_normals = nullptr;
		glm::vec2* m_texCoords = nullptr;
		Color* m_vertexColors = nullptr;
		unsigned int* m_triangleIndices = nullptr;
		unsigned int m_numVertices = 0;
		unsigned int m_numIndices = 0;
	};

}