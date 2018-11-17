#pragma once
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include "Color.h"
#include "GameComponent.h"
#include "Mesh.h"

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
		void renderMesh(Rendering::MatrixStack* const& matrixStack, Rendering::Material* const& material, Rendering::Projection const& projection, Transform* const& projectionTransform);
		unsigned int getNumVertices() const { return m_numVertices; }
		unsigned int getNumIndices() const { return m_numIndices; }

		virtual std::vector<glm::vec3> generateVertices() = 0;
		virtual std::vector<glm::vec2> generateTexCoords() { return std::vector<glm::vec2>(); }
		virtual std::vector<Color> generateVertexColors() { return std::vector<Color>(); }
		virtual std::vector<unsigned int> generateTriangleIndices() = 0;
		virtual std::vector<glm::vec3> generateTangents() { return std::vector<glm::vec3>(); }
		virtual std::vector<glm::vec3> generateBitangents() { return std::vector<glm::vec3>(); }
		virtual std::vector<glm::vec3> generateNormals() { return std::vector<glm::vec3>(); }
		virtual unsigned int generateNumVertices() { return 0; }
		virtual unsigned int generateNumIndices() { return 0; }

	private:
		Rendering::Mesh * m_mesh;
		Rendering::Material * m_material;
		Rendering::MeshComponents m_dirtyComponents;
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_tangents;
		std::vector<glm::vec3> m_bitangents;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_texCoords;
		std::vector<Color> m_vertexColors;
		std::vector<unsigned int> m_triangleIndices;
		unsigned int m_numVertices = 0;
		unsigned int m_numIndices = 0;
	};

}