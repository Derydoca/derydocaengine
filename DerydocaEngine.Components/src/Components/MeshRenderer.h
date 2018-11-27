#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine {
	namespace Components {
		class Camera;
		struct Transform;
	}
	namespace Rendering {
		class Material;
		class Mesh;
	}
}

namespace DerydocaEngine::Components
{

	class MeshRenderer : public GameComponent, SelfRegister<MeshRenderer>
	{
	public:
		GENINSTANCE(MeshRenderer)
			MeshRenderer();
		MeshRenderer(std::shared_ptr<Rendering::Mesh> const& mesh, Rendering::Material* const& material);
		~MeshRenderer();
		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack);
		virtual void renderMesh(std::shared_ptr<Rendering::MatrixStack> const matrixStack, Rendering::Material* const& material, Rendering::Projection const& projection, Transform* const& projectionTransform);
		Rendering::Material* getMaterial() { return m_material; }
		Camera* getMeshRendererCamera() { return m_meshRendererCamera; }

		void deserialize(YAML::Node const& compNode);

		void init();
		void setMesh(std::shared_ptr<Rendering::Mesh> const& mesh) { m_mesh = mesh; }
		void setMaterial(Rendering::Material* const& material) { m_material = material; }
	private:
		std::shared_ptr<Rendering::Mesh> m_mesh;
		Rendering::Material* m_material;
		Camera* m_meshRendererCamera;
	};

}
