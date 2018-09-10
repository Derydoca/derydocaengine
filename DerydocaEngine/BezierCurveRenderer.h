#pragma once
#include <GL/glew.h>
#include "GameComponent.h"
#include "glm\glm.hpp"
#include "material.h"

class BezierCurveRenderer : public GameComponent
{
public:
	GENINSTANCE(BezierCurveRenderer);

	BezierCurveRenderer();
	~BezierCurveRenderer();

	virtual void init();
	virtual void deserialize(YAML::Node const& compNode);
	void render(MatrixStack* const& matrixStack);
private:
	float m_controlPoints[8];
	GLuint m_vao;
	GLuint m_vbo;
	Material* m_material;
	int m_numSegments = 8;
	int m_numStrips = 1;
	Color m_lineColor;
};

