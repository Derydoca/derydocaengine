#pragma once
#include <string>
#include "GameComponent.h"
#include "Material.h"
#include "FontFace.h"
#include "MeshRenderer.h"

class TextRenderer : public GameComponent
{
public:
	GENINSTANCE(TextRenderer);

	TextRenderer();
	~TextRenderer();

	virtual void init();
	virtual void postInit();
	virtual void update(float deltaTime);
	virtual void render(MatrixStack* matrixStack);
	virtual void deserialize(YAML::Node compNode);

	void setText(string text) { m_text = text; }
	void updateText();
private:
	Material* m_material;
	FontFace* m_fontFace;
	vec3* m_verts;
	unsigned int* m_indices;
	vec2* m_uvs;
	string m_text = "Text";
	ivec2 m_texSize;
	Mesh m_mesh;
	float m_fontSize;
	vec2 m_bounds = vec2(5.0f, 5.0f);

	MeshRenderer* m_meshRenderer;
};

