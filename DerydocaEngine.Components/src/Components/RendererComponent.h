#pragma once
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include "Color.h"
#include "Components\GameComponent.h"
#include "Rendering\Mesh.h"

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

		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack);
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			Rendering::Material*& material,
			const Rendering::Projection& projection,
			const Transform*& projectionTransform
		);
		int getNumVertices() const { return static_cast<int>(m_vertices.size()); }
		int getNumIndices() const { return static_cast<int>(m_triangleIndices.size()); }

		virtual std::vector<glm::vec3> generateVertices() = 0;
		virtual std::vector<glm::vec2> generateTexCoords() { return std::vector<glm::vec2>(); }
		virtual std::vector<Color> generateVertexColors() { return std::vector<Color>(); }
		virtual std::vector<unsigned int> generateTriangleIndices() = 0;
		virtual std::vector<glm::vec3> generateTangents() { return std::vector<glm::vec3>(); }
		virtual std::vector<glm::vec3> generateBitangents() { return std::vector<glm::vec3>(); }
		virtual std::vector<glm::vec3> generateNormals() { return std::vector<glm::vec3>(); }

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
	};

}