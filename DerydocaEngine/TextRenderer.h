#pragma once
#include <string>
#include "GameComponent.h"
#include "Material.h"
#include "FontFace.h"
#include "MeshRenderer.h"

enum OverflowWrap
{
	Normal = 0,
	BreakWord = 1,
	BreakAll = 2,
};

enum TextAlign
{
	Start = 0,
	Center = 1,
	End = 2,
	Justify = 3,
};

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
	vec2 m_bounds;
	Color* m_vertexColors;
	Color m_textColor;
	MeshRenderer* m_meshRenderer;
	OverflowWrap m_overflowWrap;
	TextAlign m_horizontalAlign;
	TextAlign m_verticalAlign;

	int findNextBreakChar(const char* str, int startIndex);
	int findPrevBreakChar(const char* str, int startIndex);
};

