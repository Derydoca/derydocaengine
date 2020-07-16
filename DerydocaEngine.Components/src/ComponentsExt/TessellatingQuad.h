#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Ext
{

	class TessellatingQuad : public Components::GameComponent, Components::SelfRegister<TessellatingQuad>
	{
	public:
		GENINSTANCE(TessellatingQuad);
		SERIALIZE_FUNC_DEFINITIONS;

		TessellatingQuad();
		~TessellatingQuad();

		virtual void init();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
		virtual void update(const float deltaTime);

		void updateMaterial();

		int& getInner() { return m_Inner; }
		int& getOuter() { return m_Outer; }
	private:
		float m_ControlPoints[8];
		int m_Inner = 4;
		int m_Outer = 4;
		ResourceRef<Resources::MaterialResource> m_Material;

		unsigned int m_VAO;
		unsigned int m_VBO;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::TessellatingQuad, 0);
