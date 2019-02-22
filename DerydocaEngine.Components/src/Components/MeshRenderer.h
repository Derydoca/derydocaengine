#pragma once
#include "Components\GameComponent.h"
#include "Resources\MeshResource.h"
#include "Resources\MaterialResource.h"

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
		GENINSTANCE(MeshRenderer);
		MeshRenderer();
		MeshRenderer(std::shared_ptr<Resources::MeshResource> mesh, std::shared_ptr<Resources::MaterialResource> material);
		~MeshRenderer();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Transform> projectionTransform
		);
		std::shared_ptr<Rendering::Material> getMaterial() { return std::static_pointer_cast<Rendering::Material>(m_material->getResourceObjectPointer()); }
		std::shared_ptr<Resources::MaterialResource> getMaterialResource() { return m_material; }
		std::shared_ptr<Rendering::Mesh> getMesh() { return m_mesh != nullptr ? std::static_pointer_cast<Rendering::Mesh>(m_mesh->getResourceObjectPointer()) : nullptr; }
		std::shared_ptr<Resources::MeshResource> getMeshResource() { return m_mesh; }
		std::shared_ptr<Camera> getMeshRendererCamera() { return m_meshRendererCamera; }

		virtual void deserialize(const YAML::Node& compNode);
		virtual void init();
		virtual void preDestroy();

		void setMesh(std::shared_ptr<Resources::MeshResource> const& mesh) { m_mesh = mesh; }
		void setMaterial(std::shared_ptr<Resources::MaterialResource> const& material) { m_material = material; }
	private:
		std::shared_ptr<Resources::MeshResource> m_mesh;
		std::shared_ptr<Resources::MaterialResource> m_material;
		std::shared_ptr<Camera> m_meshRendererCamera;
	};

}
