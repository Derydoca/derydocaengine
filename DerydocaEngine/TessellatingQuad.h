#pragma once
#include <GL\glew.h>
#include "GameComponent.h"
#include "Material.h"

class TessellatingQuad : public GameComponent
{
public:
	GENINSTANCE(TessellatingQuad);

	TessellatingQuad();
	~TessellatingQuad();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);
	void render(MatrixStack* matrixStack);
private:
	float m_controlPoints[8];
	GLuint m_vao;
	GLuint m_vbo;
	Material* m_material;
	int m_inner = 4;
	int m_outer = 4;
	Color m_lineColor;
};

