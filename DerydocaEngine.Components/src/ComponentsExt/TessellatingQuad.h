#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class TessellatingQuad : public Components::GameComponent, Components::SelfRegister<TessellatingQuad>
	{
	public:
		GENINSTANCE(TessellatingQuad);

		TessellatingQuad();
		~TessellatingQuad();

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		int m_inner = 4;
		int m_outer = 4;
		float m_controlPoints[8];
		std::shared_ptr<Rendering::Material> m_material;
	};

}
