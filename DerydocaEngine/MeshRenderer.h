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
		MeshRenderer(Mesh* const& mesh, Material* const& material);
		~MeshRenderer();
		void render(MatrixStack* const& matrixStack);
		void renderMesh(MatrixStack* const& matrixStack, Material* const& material, Projection const& projection, Transform* const& projectionTransform);
		Material* getMaterial() { return m_material; }
		Camera* getMeshRendererCamera() { return m_meshRendererCamera; }

		void deserialize(YAML::Node const& compNode);

		void init();
		void setMesh(Mesh* const& mesh) { m_mesh = mesh; }
		void setMaterial(Material* const& material) { m_material = material; }
	private:
		Mesh * m_mesh;
		Material* m_material;
		Camera* m_meshRendererCamera;
	};

}
