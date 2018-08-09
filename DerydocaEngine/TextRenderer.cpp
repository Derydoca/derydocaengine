#include "TextRenderer.h"
#include "TexturePackerImage.h"
#include "YamlTools.h"

const int CARRIAGE_RETURN_CHAR = 13;
const int DEL_CHAR = 127;

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

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::init()
{
	m_meshRenderer = getComponent<MeshRenderer>();
}

void TextRenderer::postInit()
{
	if (m_fontFace && m_meshRenderer)
	{
		Texture* fontTexture = m_fontFace->getTexture();
		m_meshRenderer->getMaterial()->setTexture("CharacterSheet", fontTexture);
		m_texSize = ivec2(fontTexture->getWidth(), fontTexture->getHeight());
	}

	updateText();
}

void TextRenderer::update(float deltaTime)
{
}

void TextRenderer::render(MatrixStack * matrixStack)
{
	m_meshRenderer->render(matrixStack);
}

void TextRenderer::deserialize(YAML::Node compNode)
{
	YAML::Node textNode = compNode["text"];
	if (textNode)
	{
		m_text = textNode.as<string>();
	}

	YAML::Node boundsNode = compNode["bounds"];
	if (boundsNode)
	{
		m_bounds = boundsNode.as<vec2>();
	}

	YAML::Node colorNode = compNode["color"];
	if (colorNode)
	{
		m_textColor = colorNode.as<Color>();
	}

	YAML::Node overflowWrapNode = compNode["overflowWrap"];
	if (overflowWrapNode)
	{
		m_overflowWrap = static_cast<OverflowWrap>(overflowWrapNode.as<int>());
	}

	YAML::Node alignNode = compNode["align"];
	if (alignNode)
	{
		m_align = static_cast<TextAlign>(alignNode.as<int>());
	}

	auto fontResource = getResource<Resource*>(compNode, "font");
	if (fontResource->getType() == ResourceType::FontResourceType) {
		// Load the font size from the file
		float fontSize = 16.0f;
		YAML::Node fontSizeNode = compNode["fontSize"];
		if (fontSizeNode)
		{
			fontSize = fontSizeNode.as<float>();
		}

		m_fontFace = new FontFace();
		m_fontFace->setFontSize(fontSize);
		m_fontFace->loadFromFontFile(fontResource->getSourceFilePath());
	}
	else if (fontResource->getType() == ResourceType::RasterFontResourceType)
	{
		m_fontFace = new FontFace();
		m_fontFace->loadFromSerializedFile(fontResource->getSourceFilePath());
	}
}

void TextRenderer::updateText()
{
	// If no texture exists, return prematurely
	if (m_texSize.x == 0 || m_texSize.y == 0)
	{
		return;
	}

	// Store the current pen position
	float penX = 0.0f;
	float penY = 0.0f;

	// Store a character array of the size of the string so that we can store only the characters that are output to the screen
	char* filteredText = new char[m_text.length()];

	// Store a vector of line extent information to store the start and end of each line in the filtered string
	vector<LineProperties*> allLineProperties;

	// Keep track of the head of where we are writing to the filtered string
	int filteredTextIndex = 0;

	// Create a line extent object to be inserted into the lineExtents array when a full line is completed
	LineProperties* lineProperties = new LineProperties(0);

	bool continueLookingForLines = true;
	int sourceTextIndex = 0;
	float currentLineWidth = 0.0f;
	float currentLineStartAdjust = 0.0f;
	float lastBreakableWidth = 0.0f;
	int lastBreakableSourceIndex = 0;
	int lastBreakableFilteredIndex = 0;
	bool lookingForNextLineBreak = false;
	int newLineSourceTextIndex = 0;
	int newLineFilteredTextIndex = 0;

	while (continueLookingForLines)
	{
		int endChar = 0;
		bool addChar = true;
		char c = m_text[sourceTextIndex];
		
		if (c == CARRIAGE_RETURN_CHAR)
		{
			endChar = filteredTextIndex;
			addChar = false;
			newLineSourceTextIndex = sourceTextIndex;
			newLineFilteredTextIndex = filteredTextIndex;
		}
		else if (c == 0)
		{
			// Force ending the line when we are processing the last character in the string
			endChar = filteredTextIndex;
			addChar = false;
			continueLookingForLines = false;
			newLineFilteredTextIndex = filteredTextIndex;
			newLineSourceTextIndex = m_text.size();
		}


		if (addChar)
		{
			TexturePackerImage img = m_fontFace->getCharData(c);
			bool isWhitespace = img.getWidth() == 0.0f;
			bool isBreakable = c == ' ' || c == '-';

			if (isBreakable)
			{
				lastBreakableFilteredIndex = filteredTextIndex;
				lastBreakableSourceIndex = sourceTextIndex;
				lastBreakableWidth = currentLineWidth;

				if (lookingForNextLineBreak)
				{
					endChar = filteredTextIndex + 1;
					newLineSourceTextIndex = sourceTextIndex + 1;
					newLineFilteredTextIndex = filteredTextIndex + 1;
				}
			}

			if (isWhitespace && currentLineWidth == 0.0f)
			{
				currentLineStartAdjust -= img.getAdvanceX();
			}
			
			if (!lookingForNextLineBreak && m_bounds.x > 0 && (currentLineWidth + img.getAdvanceX() + currentLineStartAdjust) > m_bounds.x)
			{
				if (m_overflowWrap == OverflowWrap::BreakAll)
				{
					endChar = filteredTextIndex;
					newLineSourceTextIndex = sourceTextIndex;
					newLineFilteredTextIndex = filteredTextIndex;
				}
				else
				{
					if (lastBreakableFilteredIndex > lineProperties->getStart())
					{
						endChar = lastBreakableFilteredIndex;
						currentLineWidth = lastBreakableWidth;
						newLineSourceTextIndex = lastBreakableSourceIndex;
						newLineFilteredTextIndex = lastBreakableFilteredIndex;
					}
					else if (m_overflowWrap == OverflowWrap::Normal)
					{
						lookingForNextLineBreak = true;
						currentLineWidth += img.getAdvanceX();
					}
					else
					{
						endChar = filteredTextIndex;
						newLineSourceTextIndex = sourceTextIndex;
						newLineFilteredTextIndex = filteredTextIndex;
					}
				}
			}
			else
			{
				currentLineWidth += img.getAdvanceX();
			}

			if (endChar > 0 && isWhitespace)
			{
				currentLineWidth -= img.getAdvanceX();
			}
		}

		if (addChar)
		{
			filteredText[filteredTextIndex++] = c;
		}

		if (endChar > 0)
		{
			lineProperties->setEnd(endChar);
			lineProperties->setWidth(currentLineWidth);
			lineProperties->setStartAdjust(currentLineStartAdjust);
			allLineProperties.push_back(lineProperties);

			lineProperties = new LineProperties(endChar);
			currentLineWidth = 0.0f;
			currentLineStartAdjust = 0.0f;
			endChar = 0;
			lookingForNextLineBreak = false;

			sourceTextIndex = newLineSourceTextIndex;
			filteredTextIndex = newLineFilteredTextIndex;
		}
		else
		{
			sourceTextIndex++;
		}
	}

	delete[] m_verts;
	delete[] m_uvs;
	delete[] m_indices;
	delete[] m_vertexColors;

	int charCount = allLineProperties.back()->getEnd();
	int vertCount = charCount * 4;
	int indicesCount = charCount * 6;

	m_verts = new vec3[vertCount];
	m_uvs = new vec2[vertCount];
	m_indices = new unsigned int[indicesCount];
	m_vertexColors = new Color[vertCount];

	int charQuadIndex = 0;
	for (int lineIndex = 0; lineIndex < allLineProperties.size(); lineIndex++)
	{
		LineProperties* extent = allLineProperties[lineIndex];
		float extraCharAdvance = 0.0f;

		switch (m_align)
		{
		case Center:
			penX = (m_bounds.x - extent->getWidth() - extent->getStartAdjust()) / 2.0f;
			break;
		case Right:
			penX = m_bounds.x - extent->getWidth() - extent->getStartAdjust();
			break;
		case Justify:
			penX = 0.0f;
			if (m_bounds.x != 0)
			{
				int textLength = (extent->getEnd() - extent->getStart());
				if (textLength != 0)
				{
					extraCharAdvance = (m_bounds.x - extent->getWidth()) / textLength;
				}
			}
			break;
		default:
			penX = 0.0f;
			break;
		}

		penX += extent->getStartAdjust();

		for (int charIndex = extent->getStart(); charIndex < extent->getEnd(); charIndex++)
		{
			TexturePackerImage img = m_fontFace->getCharData(filteredText[charIndex]);
			Rect rect = img.getTexSheetPosition();
			float minx = rect.getX();
			float maxx = rect.getDX();
			float miny = rect.getY();
			float maxy = rect.getDY();

			// Set the UV positions
			m_uvs[charQuadIndex * 4 + 1] = vec2(minx, maxy);
			m_uvs[charQuadIndex * 4 + 0] = vec2(minx, miny);
			m_uvs[charQuadIndex * 4 + 3] = vec2(maxx, miny);
			m_uvs[charQuadIndex * 4 + 2] = vec2(maxx, maxy);

			// Set the indices to draw the two triangles
			m_indices[charQuadIndex * 6 + 0] = charQuadIndex * 4 + 0;
			m_indices[charQuadIndex * 6 + 1] = charQuadIndex * 4 + 1;
			m_indices[charQuadIndex * 6 + 2] = charQuadIndex * 4 + 2;
			m_indices[charQuadIndex * 6 + 3] = charQuadIndex * 4 + 0;
			m_indices[charQuadIndex * 6 + 4] = charQuadIndex * 4 + 2;
			m_indices[charQuadIndex * 6 + 5] = charQuadIndex * 4 + 3;

			// Set the vertex positions
			float charXMin = penX + img.getBearingX();
			float charXMax = penX + img.getBearingX() + img.getSizeX();

			float charYMax = penY - img.getSizeY() + img.getBearingY();
			float charYMin = penY + img.getBearingY();
			m_verts[charQuadIndex * 4 + 0] = vec3(charXMin, charYMin, 0);
			m_verts[charQuadIndex * 4 + 1] = vec3(charXMin, charYMax, 0);
			m_verts[charQuadIndex * 4 + 2] = vec3(charXMax, charYMax, 0);
			m_verts[charQuadIndex * 4 + 3] = vec3(charXMax, charYMin, 0);

			// Set the vertex colors
			m_vertexColors[charQuadIndex * 4 + 0] = m_textColor;
			m_vertexColors[charQuadIndex * 4 + 1] = m_textColor;
			m_vertexColors[charQuadIndex * 4 + 2] = m_textColor;
			m_vertexColors[charQuadIndex * 4 + 3] = m_textColor;

			penX += img.getAdvanceX() + extraCharAdvance;
			penY += img.getAdvanceY();

			charQuadIndex++;
		}

		penY -= m_fontFace->getLineHeight();

		delete extent;
	}

	allLineProperties.clear();
	delete[] filteredText;

	m_mesh.load(vertCount, m_verts, nullptr, m_uvs, m_indices, indicesCount);
	m_mesh.loadVertexColorBuffer(vertCount, m_vertexColors);
	m_meshRenderer->setMesh(&m_mesh);
}

int TextRenderer::findNextBreakChar(const char* str, int startIndex)
{
	int i = startIndex;
	bool continueLooking = true;
	while (continueLooking)
	{
		char c = str[i];
		if (c == 0 || c == ' ' || c == '-')
		{
			return i;
		}
		i++;
	}
	return i - 1;
}

int TextRenderer::findPrevBreakChar(const char* str, int startIndex)
{
	for (int i = startIndex; i > 0; i--)
	{
		char c = str[i];

		if (c == ' ' || c == '-')
		{
			return i + 1;
		}
	}

	return 0;
}
