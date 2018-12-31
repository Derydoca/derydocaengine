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
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Transform> projectionTransform
		);
		virtual void update(const float deltaTime) { m_time += deltaTime; }
		std::shared_ptr<Rendering::Material> getMaterial() { return m_material; }
		std::shared_ptr<Camera> getSkinnedMeshRendererCamera() { return m_SkinnedMeshRendererCamera; }

		void deserialize(const YAML::Node& compNode);

		void init();
		void setMaterial(std::shared_ptr<Rendering::Material> const& material) { m_material = material; }
	private:
		std::shared_ptr<Rendering::Mesh> m_mesh;
		std::shared_ptr<Rendering::Material> m_material;
		std::shared_ptr<Camera> m_SkinnedMeshRendererCamera;
		std::shared_ptr<Animation::AnimationData> m_animation;
		float m_time;
		std::vector<glm::mat4> m_boneMatrices;
	};

}
