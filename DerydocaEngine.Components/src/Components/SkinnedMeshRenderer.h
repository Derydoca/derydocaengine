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

		void deserialize(const YAML::Node& compNode) override;
		void init() override;
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) override;
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Transform> projectionTransform
		) override;
		virtual void update(const float deltaTime) override;
		
		bool isFullyConfigured() const { return m_animation && m_material && m_mesh; };

		void setAnimationResource(std::shared_ptr<Resources::AnimationResource> animationResource)
		{
			m_animation = animationResource;
			m_dirty = true;
		}
		void setMaterialResource(std::shared_ptr<Resources::MaterialResource> materialResource)
		{
			m_material = materialResource;
		}
		void setMeshResource(std::shared_ptr<Resources::MeshResource> meshResource)
		{
			m_mesh = meshResource;
			m_dirty = true;
		}
		
		std::shared_ptr<Animation::AnimationData> getAnimation() const { return std::static_pointer_cast<Animation::AnimationData>(m_animation->getResourceObjectPointer()); }
		std::shared_ptr<Rendering::Material> getMaterial() { return m_material ? std::static_pointer_cast<Rendering::Material>(m_material->getResourceObjectPointer()) : nullptr; }
		std::shared_ptr<Rendering::Mesh> getMesh() const { return m_mesh ? std::static_pointer_cast<Rendering::Mesh>(m_mesh->getResourceObjectPointer()) : nullptr; }

		std::shared_ptr<Resources::AnimationResource> getAnimationResource() const { return m_animation; }
		std::shared_ptr<Resources::MaterialResource> getMaterialResource() { return m_material; }
		std::shared_ptr<Resources::MeshResource> getMeshResource() const { return m_mesh; }

	private:
		void fixDirtyStatus();

	private:
		float m_time;
		bool m_dirty;
		std::shared_ptr<Resources::AnimationResource> m_animation;
		std::shared_ptr<Resources::MaterialResource> m_material;
		std::shared_ptr<Resources::MeshResource> m_mesh;
		std::vector<glm::mat4> m_boneMatrices;
	};

}
