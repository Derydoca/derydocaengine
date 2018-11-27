#pragma once
#include "Animation\AnimationData.h"
#include "Components\GameComponent.h"
#include "Animation\Skeleton.h"

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
		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack);
		virtual void renderMesh(std::shared_ptr<Rendering::MatrixStack> const matrixStack, Rendering::Material* const& material, Rendering::Projection const& projection, Transform* const& projectionTransform);
		virtual void update(float const& deltaTime) { m_time += deltaTime; }
		Rendering::Material* getMaterial() { return m_material; }
		Camera* getSkinnedMeshRendererCamera() { return m_SkinnedMeshRendererCamera; }

		void deserialize(YAML::Node const& compNode);

		void init();
		void setMaterial(Rendering::Material* const& material) { m_material = material; }
	private:
		std::shared_ptr<Rendering::Mesh> m_mesh;
		Rendering::Material* m_material;
		Camera* m_SkinnedMeshRendererCamera;
		std::shared_ptr<Animation::AnimationData> m_animation;
		float m_time = 0.0f;
		std::vector<glm::mat4> m_boneMatrices;
	};

}
