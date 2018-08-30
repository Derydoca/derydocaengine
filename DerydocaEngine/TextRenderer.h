#pragma once
#include <string>
#include "RendererComponent.h"
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

class TextRenderer : public RendererComponent
{
public:
	GENINSTANCE(TextRenderer);

	virtual void postInit();
	virtual void deserialize(YAML::Node compNode);

	void setText(string text)
	{
		m_text = text;
		m_lines = processTextToLines(m_text, m_overflowWrap, m_fontFace, m_bounds.x, m_filteredText);
		int newVertCount = generateNumVertices();
		int currentVertCount = getNumVertices();
		if (newVertCount != currentVertCount)
		{
			markComponentAsDirty(DIRTY_COMPONENTS_ON_INDICES_CHANGED);
		}
		markComponentAsDirty(DIRTY_COMPONENTS_ON_TEXT_CHANGE);
	}
	string getText() { return m_text; }
	void setColor(Color color)
	{
		if (m_textColor == color)
		{
			return;
		}

		m_textColor = color;
		markComponentAsDirty(MeshComponents::Colors);
	}

	virtual vec3* generateVertices();
	virtual vec2* generateTexCoords();
	virtual Color* generateVertexColors();
	virtual unsigned int* generateTriangleIndices();
	virtual unsigned int generateNumVertices()
	{
		if (m_lines.size() == 0)
		{
			return 0;
		}
		int charCount = m_lines.back()->getEnd();
		return charCount * 4;
	}
	virtual unsigned int generateNumIndices()
	{
		if (m_lines.size() == 0)
		{
			return 0;
		}
		int charCount = m_lines.back()->getEnd();
		return charCount * 6;
	}

private:
	const MeshComponents DIRTY_COMPONENTS_ON_TEXT_CHANGE = (MeshComponents)(MeshComponents::Positions | MeshComponents::TexCoords);
	const MeshComponents DIRTY_COMPONENTS_ON_INDICES_CHANGED = (MeshComponents)(MeshComponents::Colors | MeshComponents::Indices);

	Material* m_material;
	FontFace* m_fontFace;
	string m_text = "Text";
	vec2 m_bounds;
	Color m_textColor;
	OverflowWrap m_overflowWrap;
	TextAlign m_horizontalAlign;
	TextAlign m_verticalAlign;
	vector<LineProperties*> m_lines;
	char* m_filteredText;
	bool m_textDirty = true;

	static void calculateVerticalAlignmentProperties(TextAlign alignment, int numberOfLines, float verticalBoundSize, float fontLineHeight, float* penY, float* newLineHeight);
	static void calculateHorizontalAlignmentProperties(TextAlign alignment, float horizontalBoundSize, float lineWidth, int numChars, float lineStartAdjust, float* penX, float* extraCharAdvance);
	static vector<LineProperties*> processTextToLines(string text, OverflowWrap overflowWrap, FontFace* fontFace, float horizontalBoundSize, char*& filteredText);
};

