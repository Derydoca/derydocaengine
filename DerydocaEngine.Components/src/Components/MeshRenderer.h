#pragma once
#include "Components\GameComponent.h"
#include "Resources\MeshResource.h"
#include "Resources\MaterialResource.h"
#include "ResourceRef.h"

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
		std::shared_ptr<Rendering::Material> getMaterial() { return std::static_pointer_cast<Rendering::Material>(m_Material->getResourceObjectPointer()); }
		std::shared_ptr<Resources::MaterialResource> getMaterialResource() { return m_Material.GetSmartPointer(); }
		std::shared_ptr<Rendering::Mesh> getMesh() { return m_Mesh.GetSmartPointer() != nullptr ? std::static_pointer_cast<Rendering::Mesh>(m_Mesh->getResourceObjectPointer()) : nullptr; }
		std::shared_ptr<Resources::MeshResource> getMeshResource() { return m_Mesh.GetSmartPointer(); }

		virtual void deserialize(const YAML::Node& compNode);
		virtual void init();
		virtual void preDestroy();

		void setMesh(std::shared_ptr<Resources::MeshResource> const& mesh) { m_Mesh.Set(mesh); }
		void setMaterial(std::shared_ptr<Resources::MaterialResource> const& material) { m_Material.Set(material); }
		void setTransparent(const bool isTransparent) { m_Transparent = isTransparent; }

		// HACK: This was added to allow serializing of ShaderSubroutineSwitcher because it loses the material ID when creating
		//  a new instance and the mesh renderer will not have a material to load after saving the scene.
		void setMaterialId(boost::uuids::uuid id) { m_Material.GetSmartPointer()->setId(id); }
		boost::uuids::uuid getMaterialId() { return m_Material.GetSmartPointer()->getId(); }

		SERIALIZE_FUNC_BASE(
			DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_Mesh),
			SERIALIZE(m_Material),
			SERIALIZE(m_Transparent)
		);
	private:
		ResourceRef<Resources::MeshResource> m_Mesh;
		ResourceRef<Resources::MaterialResource> m_Material;
		bool m_Transparent;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::MeshRenderer);