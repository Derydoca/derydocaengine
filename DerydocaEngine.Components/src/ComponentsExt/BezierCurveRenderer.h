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
		virtual void deserialize(const YAML::Node& compNode);
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		Color& getColor() { return m_lineColor; }
		int& getNumStrips() { return m_numStrips; }
		int& getNumSegments() { return m_numSegments; }
	private:
		float m_controlPoints[8];
		unsigned int m_vao;
		unsigned int m_vbo;
		std::shared_ptr<Rendering::Material> m_material;
		int m_numSegments = 8;
		int m_numStrips = 1;
		Color m_lineColor;
	};

}
