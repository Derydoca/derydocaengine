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
		virtual void deserialize(YAML::Node const& compNode);
		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack);
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			Rendering::Material*& material,
			const Rendering::Projection& projection,
			const Components::Transform*& projectionTransform
		);

		Rendering::Material* getMaterial() const { return m_material; }
	private:
		BezierPatchMesh * m_mesh;
		unsigned int m_vao;
		unsigned int m_vbo;
		Rendering::Material* m_material;
		int m_tessellationLevel = 4;
		bool m_useDynamicTessellation = false;
		float m_minDynamicTessLevel = 2;
		float m_maxDynamicTessLevel = 8;
		float m_minDynamicTessDistance = 1.0;
		float m_maxDynamicTessDistance = 10.0;
		Color m_lineColor;
	};

}
