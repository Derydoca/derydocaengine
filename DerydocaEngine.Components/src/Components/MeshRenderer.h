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
		MeshRenderer(std::shared_ptr<Rendering::Mesh> mesh, std::shared_ptr<Rendering::Material> material);
		~MeshRenderer();
		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack);
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Transform> projectionTransform
		);
		std::shared_ptr<Rendering::Material> getMaterial() { return m_material; }
		std::shared_ptr<Camera> getMeshRendererCamera() { return m_meshRendererCamera; }

		virtual void deserialize(YAML::Node const& compNode);
		virtual void init();
		virtual void preDestroy();

		void setMesh(std::shared_ptr<Rendering::Mesh> const& mesh) { m_mesh = mesh; }
		void setMaterial(std::shared_ptr<Rendering::Material> const& material) { m_material = material; }
	private:
		std::shared_ptr<Rendering::Mesh> m_mesh;
		std::shared_ptr<Rendering::Material> m_material;
		std::shared_ptr<Camera> m_meshRendererCamera;
	};

}
