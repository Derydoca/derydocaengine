#pragma once
#include "Components\GameComponent.h"
#include "glm\glm.hpp"
#include "Rendering\Material.h"
#include "Resources\ShaderResource.h"
#include "Color.h"

namespace DerydocaEngine::Ext
{

	class BezierCurveRenderer : public Components::GameComponent, Components::SelfRegister<BezierCurveRenderer>
	{
	public:
		GENINSTANCE(BezierCurveRenderer);
		SERIALIZE_FUNC_DEFINITIONS;

		BezierCurveRenderer();
		~BezierCurveRenderer();

		virtual void init();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);

		Color& getColor() { return m_LineColor; }
		int& getNumStrips() { return m_NumStrips; }
		int& getNumSegments() { return m_NumSegments; }
	private:
		float m_ControlPoints[8];
		int m_NumSegments = 8;
		int m_NumStrips = 1;
		Color m_LineColor;
		ResourceRef<Resources::ShaderResource> m_Shader;

		unsigned int m_VAO;
		unsigned int m_VBO;
		std::shared_ptr<Rendering::Material> m_Material;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::BezierCurveRenderer, 0);
