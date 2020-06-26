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

		/* Defines how a light's shadowmap is filtered between texels */
		enum ShadowMapFilterType
		{
			/* Hard shadowing with visible aliasing */
			Nearest = 0,
			/* Linearly interpolate between texels for smooth shadow edges */
			Linear = 1
		};

		Light();
		~Light();

		void setLightType(LightType const& type) { m_Type = type; }
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
		bool& getCastShadows() { return m_CastShadows; }
		Color& getColor() { return m_Color; }
		Color getColor() const { return m_Color; }
		float getSpotlightExponent() { return m_SpotlightExponent; }
		float getSpotlightCutoff() { return m_SpotlightCutoff; }
		unsigned int getShadowMap() { return m_DepthTexture; }
		bool isCastingShadows() { return m_CastShadows; }
		bool setCastingShadows(bool const& castShadows) { m_CastShadows = castShadows; }
		Rendering::Projection& getProjection() { return m_Projection; }
		Rendering::Projection getProjection() const { return m_Projection; }
		glm::mat4 getShadowMatrix();
		float& getShadowSoftness() { return m_ShadowSoftness; }
		float getShadowSoftness() const { return m_ShadowSoftness; }

		virtual void init();
		virtual void deserialize(const YAML::Node& node);
		virtual void preDestroy();

		void renderShadowMap(const std::vector<std::shared_ptr<Scenes::Scene>> scenes);

		SERIALIZE_FUNC_BASE(
			DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_Type),
			SERIALIZE(m_ShadowMapFilterType),
			SERIALIZE(m_ShadowMapSize),
			SERIALIZE(m_Color),
			SERIALIZE(m_CastShadows),
			SERIALIZE(m_SpotlightExponent),
			SERIALIZE(m_SpotlightCutoff),
			SERIALIZE(m_ShadowSoftness)
		);

	private:
		void generateShadowMap();
		int getShadowMapFilterTypeEnum();

		LightType m_Type;
		ShadowMapFilterType m_ShadowMapFilterType;
		int2 m_ShadowMapSize;
		Color m_Color;
		bool m_CastShadows;
		float m_SpotlightExponent;
		float m_SpotlightCutoff;
		float m_ShadowSoftness;

		unsigned int m_DepthTexture;
		unsigned int m_ShadowFBO;
		std::shared_ptr<Rendering::MatrixStack> m_MatrixStack;
		std::shared_ptr<Rendering::Material> m_ShadowMapMaterial;
		Rendering::Projection m_Projection;
		glm::mat4 m_ShadowBias;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::Light);
