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
		SERIALIZE_FUNC_DEFINITIONS;
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
		
		bool isFullyConfigured() const { return m_Animation && m_Material && m_Mesh; };

		void setAnimationResource(std::shared_ptr<Resources::AnimationResource> animationResource)
		{
			m_Animation = animationResource;
			m_Dirty = true;
		}
		void setMaterialResource(std::shared_ptr<Resources::MaterialResource> materialResource)
		{
			m_Material = materialResource;
		}
		void setMeshResource(std::shared_ptr<Resources::MeshResource> meshResource)
		{
			m_Mesh = meshResource;
			m_Dirty = true;
		}
		void setAnimationTime(const float animationTime) { m_Time = animationTime; }
		
		std::shared_ptr<Animation::AnimationData> getAnimation() const { return m_Animation ? std::static_pointer_cast<Animation::AnimationData>(m_Animation->getResourceObjectPointer()) : nullptr; }
		std::shared_ptr<Rendering::Material> getMaterial() { return m_Material ? std::static_pointer_cast<Rendering::Material>(m_Material->getResourceObjectPointer()) : nullptr; }
		std::shared_ptr<Rendering::Mesh> getMesh() const { return m_Mesh ? std::static_pointer_cast<Rendering::Mesh>(m_Mesh->getResourceObjectPointer()) : nullptr; }

		std::shared_ptr<Resources::AnimationResource> getAnimationResource() const { return m_Animation.GetSmartPointer(); }
		std::shared_ptr<Resources::MaterialResource> getMaterialResource() { return m_Material.GetSmartPointer(); }
		std::shared_ptr<Resources::MeshResource> getMeshResource() const { return m_Mesh.GetSmartPointer(); }

	private:
		void fixDirtyStatus();

	private:
		ResourceRef<Resources::AnimationResource> m_Animation;
		ResourceRef<Resources::MaterialResource> m_Material;
		ResourceRef<Resources::MeshResource> m_Mesh;

		float m_Time;
		bool m_Dirty;
		std::vector<glm::mat4> m_BoneMatrices;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::SkinnedMeshRenderer, 0);
