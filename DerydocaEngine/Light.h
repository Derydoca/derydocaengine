#pragma once
#include "glm\glm.hpp"
#include "GameComponent.h"
#include "Color.h"
#include "MatrixStack.h"
#include "Material.h"
#include "Transform.h"
#include "Projection.h"

class Light : public GameComponent
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

	void setLightType(LightType type) { m_lightType = type; }
	LightType getLightType() { return m_lightType; }

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void setColor(Color* color)
	{
		m_color.r = color->r;
		m_color.g = color->g;
		m_color.b = color->b;
		m_color.a = color->a;
	}
	Color getColor() { return m_color; }
	float getSpotlightExponent() { return m_spotlightExponent; }
	float getSpotlightCutoff() { return m_spotlightCutoff; }
	GLuint getShadowMap() { return m_depthTexture; }
	bool isCastingShadows() { return m_castShadows; }
	bool setCastingShadows(bool castShadows) { m_castShadows = castShadows; }
	Projection getProjection() { return m_projection; }
	glm::mat4 getShadowMatrix(mat4 objectModelMatrix);
	float getShadowSoftness() const { return m_shadowSoftness; }

	void init();
	void deserialize(YAML::Node node);

	void renderShadowMap(GameObject* gameObject);
private:
	void generateShadowMap();
	GLint getShadowMapFilterTypeEnum();

	LightType m_lightType = Point;
	Color m_color = Color(1, 1, 1, 1);
	float m_spotlightExponent;
	float m_spotlightCutoff;
	bool m_castShadows = false;
	int m_shadowMapHeight = 512;
	int m_shadowMapWidth = 512;
	GLuint m_depthTexture;
	GLuint m_shadowFBO;
	MatrixStack m_matrixStack;
	Material* m_shadowMapMaterial;
	Projection m_projection;
	glm::mat4 m_shadowBias;
	ShadowMapFilterType m_shadowMapFilterType = ShadowMapFilterType::Nearest;
	float m_shadowSoftness = 0.01f;
};

