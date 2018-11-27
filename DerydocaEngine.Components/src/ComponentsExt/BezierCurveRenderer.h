#pragma once
#include "Components\GameComponent.h"
#include "glm\glm.hpp"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class BezierCurveRenderer : public Components::GameComponent, Components::SelfRegister<BezierCurveRenderer>
	{
	public:
		GENINSTANCE(BezierCurveRenderer);

		BezierCurveRenderer();
		~BezierCurveRenderer();

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack);
	private:
		float m_controlPoints[8];
		unsigned int m_vao;
		unsigned int m_vbo;
		Rendering::Material* m_material;
		int m_numSegments = 8;
		int m_numStrips = 1;
		Color m_lineColor;
	};

}
