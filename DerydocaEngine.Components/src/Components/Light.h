#pragma once

#include <glm\mat4x4.hpp>
#include "Components\GameComponent.h"
#include "Color.h"
#include "Rendering\MatrixStack.h"
#include "Components\Transform.h"
#include "Rendering\Projection.h"
#include "Scenes\Scene.h"

namespace DerydocaEngine::Rendering {
	class Material;
}

namespace DerydocaEngine::Components
{

	class Light : public GameComponent, SelfRegister<Light>
	{
	public:
		GENINSTANCE(Light);
		SERIALIZE_FUNC_DEFINITIONS;

		/* Defines a light's type */
		enum LightType
		{
			/* A orthagonal light infinitely far away */
			Directional = 0,
			/* A light radiating in all directions */
			Point = 1,
			/* A light projected in a frustum */
			Spotlight = 2,
		};

		Light();
		~Light();

		LightType getLightType() { return m_Type; }

		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();

		void setColor(Color* const& color)
		{
			m_Color.r = color->r;
			m_Color.g = color->g;
			m_Color.b = color->b;
			m_Color.a = color->a;
		}
		Color& getColor() { return m_Color; }
		Color getColor() const { return m_Color; }
		float getSpotlightExponent() { return m_SpotlightExponent; }
		float getSpotlightCutoff() { return m_SpotlightCutoff; }

		virtual void init();
		virtual void preDestroy();


	private:
		LightType m_Type;
		Color m_Color;
		float m_SpotlightExponent;
		float m_SpotlightCutoff;

		std::shared_ptr<Rendering::MatrixStack> m_MatrixStack;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::Light, 0);
