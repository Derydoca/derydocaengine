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

struct LineProperties
{
public:
	LineProperties()
	{
		m_start = 0;
		m_end = 0;
	}

	LineProperties(int start)
	{
		m_start = start;
		m_end = start;
	}

	int getStart() const { return m_start; }
	int getEnd() const { return m_end; }
	float getWidth() const { return m_width; }
	float getStartAdjust() const { return m_startAdjust; }
	void setStart(int start) { m_start = start; }
	void setEnd(int end) { m_end = end; }
	void setWidth(float width) { m_width = width; }
	void setStartAdjust(float startAdjust) { m_startAdjust = startAdjust; }
private:
	int m_start;
	int m_end;
	float m_width;
	float m_startAdjust;
};

class TextRenderer : public GameComponent
{
public:
	GENINSTANCE(TextRenderer);

	TextRenderer();
	~TextRenderer();

	virtual void init();
	virtual void postInit();
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
	Mesh m_mesh;
	vec2 m_bounds;
	Color* m_vertexColors;
	Color m_textColor;
	MeshRenderer* m_meshRenderer;
	OverflowWrap m_overflowWrap;
	TextAlign m_horizontalAlign;
	TextAlign m_verticalAlign;

	static void calculateVerticalAlignmentProperties(TextAlign alignment, int numberOfLines, float verticalBoundSize, float fontLineHeight, float* penY, float* newLineHeight);
	static void calculateHorizontalAlignmentProperties(TextAlign alignment, float horizontalBoundSize, float lineWidth, int numChars, float lineStartAdjust, float* penX, float* extraCharAdvance);
	static vector<LineProperties*> processTextToLines(string text, OverflowWrap overflowWrap, FontFace* fontFace, float horizontalBoundSize, char*& filteredText);
};

