#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class TessellatingQuad : public Components::GameComponent, Components::SelfRegister<TessellatingQuad>
	{
	public:
		GENINSTANCE(TessellatingQuad);

		TessellatingQuad() {}
		~TessellatingQuad() {}

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
	private:
		float m_controlPoints[8];
		unsigned int m_vao;
		unsigned int m_vbo;
		std::shared_ptr<Rendering::Material> m_material;
		int m_inner = 4;
		int m_outer = 4;
		Color m_lineColor;
	};

}
