#pragma once
#include "Animation\AnimationData.h"
#include "Components\GameComponent.h"
#include "Animation\Skeleton.h"
#include "Resources\AnimationResource.h"
#include "Resources\MaterialResource.h"
#include "Resources\MeshResource.h"

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

	class SkinnedMeshRenderer : public GameComponent, SelfRegister<SkinnedMeshRenderer>
	{
	public:
		GENINSTANCE(SkinnedMeshRenderer)
		SkinnedMeshRenderer();
		~SkinnedMeshRenderer();

		void init() override;
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) override;
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Transform> projectionTransform
		) override;
		virtual void update(const float deltaTime) override;
		
		void setAnimationResource(std::shared_ptr<Resources::AnimationResource> animationResource) { m_animation = animationResource; }
		void setMaterialResource(std::shared_ptr<Resources::MaterialResource> materialResource) { m_material = materialResource; }
		void setMeshResource(std::shared_ptr<Resources::MeshResource> meshResource) { m_mesh = meshResource; }
		
		std::shared_ptr<Animation::AnimationData> getAnimation() const { return std::static_pointer_cast<Animation::AnimationData>(m_animation->getResourceObjectPointer()); }
		std::shared_ptr<Rendering::Material> getMaterial() { return m_material ? std::static_pointer_cast<Rendering::Material>(m_material->getResourceObjectPointer()) : nullptr; }
		std::shared_ptr<Rendering::Mesh> getMesh() const { return m_mesh ? std::static_pointer_cast<Rendering::Mesh>(m_mesh->getResourceObjectPointer()) : nullptr; }

		std::shared_ptr<Resources::AnimationResource> getAnimationResource() const { return m_animation; }
		std::shared_ptr<Resources::MaterialResource> getMaterialResource() { return m_material; }
		std::shared_ptr<Resources::MeshResource> getMeshResource() const { return m_mesh; }

		void deserialize(const YAML::Node& compNode);

	private:
		float m_time;
		std::shared_ptr<Resources::AnimationResource> m_animation;
		std::shared_ptr<Resources::MaterialResource> m_material;
		std::shared_ptr<Resources::MeshResource> m_mesh;
		std::vector<glm::mat4> m_boneMatrices;
	};

}
