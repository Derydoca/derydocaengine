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

		TessellatedMeshRenderer() {}
		~TessellatedMeshRenderer() {}

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
		std::shared_ptr<BezierPatchMesh> m_mesh;
		unsigned int m_vao;
		unsigned int m_vbo;
		std::shared_ptr<Rendering::Material> m_material;
		int m_tessellationLevel = 4;
		bool m_useDynamicTessellation = false;
		int m_minDynamicTessLevel = 2;
		int m_maxDynamicTessLevel = 8;
		float m_minDynamicTessDistance = 1.0;
		float m_maxDynamicTessDistance = 10.0;
		Color m_lineColor;
	};

}
