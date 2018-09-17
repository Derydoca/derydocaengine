#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Transform.h"

namespace DerydocaEngine::Components
{

	class MeshRenderer : public GameComponent
	{
	public:
		GENINSTANCE(MeshRenderer)
			MeshRenderer();
		MeshRenderer(Rendering::Mesh* const& mesh, Rendering::Material* const& material);
		~MeshRenderer();
		void render(Rendering::MatrixStack* const& matrixStack);
		void renderMesh(Rendering::MatrixStack* const& matrixStack, Rendering::Material* const& material, Rendering::Projection const& projection, Transform* const& projectionTransform);
		Rendering::Material* getMaterial() { return m_material; }
		DerydocaEngine::Components::Camera* getMeshRendererCamera() { return m_meshRendererCamera; }

		void deserialize(YAML::Node const& compNode);

		void init();
		void setMesh(Rendering::Mesh* const& mesh) { m_mesh = mesh; }
		void setMaterial(Rendering::Material* const& material) { m_material = material; }
	private:
		Rendering::Mesh * m_mesh;
		Rendering::Material* m_material;
		DerydocaEngine::Components::Camera* m_meshRendererCamera;
	};

}
