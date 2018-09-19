#pragma once
#include "GameComponent.h"
#include <GL\glew.h>
#include "Material.h"
#include "BezierPatchMesh.h"

namespace DerydocaEngine::Ext
{

	class TessellatedMeshRenderer : public Components::GameComponent
	{
	public:
		GENINSTANCE(TessellatedMeshRenderer);

		TessellatedMeshRenderer() {}
		~TessellatedMeshRenderer() {}

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		void render(Rendering::MatrixStack* const& matrixStack);
		void renderMesh(Rendering::MatrixStack* const& matrixStack, Rendering::Material* const& material, Rendering::Projection const& projection, Transform* const& projectionTransform);

		Rendering::Material* getMaterial() const { return m_material; }
	private:
		DerydocaEngine::Ext::BezierPatchMesh * m_mesh;
		GLuint m_vao;
		GLuint m_vbo;
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
