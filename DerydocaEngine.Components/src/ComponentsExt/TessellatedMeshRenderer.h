#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"
#include "Ext\BezierPatchMesh.h"

namespace DerydocaEngine::Ext
{

	class TessellatedMeshRenderer : public Components::GameComponent, private Components::SelfRegister<TessellatedMeshRenderer>
	{
	public:
		GENINSTANCE(TessellatedMeshRenderer);

		TessellatedMeshRenderer();
		~TessellatedMeshRenderer();

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Components::Transform> projectionTransform
		);
		virtual void update(const float deltaTime);

		std::shared_ptr<Rendering::Material> getMaterial() const { return m_material; }
		bool& getUseDynamicTessellation() { return m_useDynamicTessellation; }
		int& getTessellationLevel() { return m_tessellationLevel; }
		int& getMinDynamicTessLevel() { return m_minDynamicTessLevel; }
		int& getMaxDynamicTessLevel() { return m_maxDynamicTessLevel; }
		float& getMinDynamicTessDistance() { return m_minDynamicTessDistance; }
		float& getMaxDynamicTessDistance() { return m_maxDynamicTessDistance; }
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		bool m_useDynamicTessellation;
		int m_tessellationLevel;
		int m_minDynamicTessLevel;
		int m_maxDynamicTessLevel;
		float m_minDynamicTessDistance;
		float m_maxDynamicTessDistance;
		std::shared_ptr<BezierPatchMesh> m_mesh;
		std::shared_ptr<Rendering::Material> m_material;
	};

}
