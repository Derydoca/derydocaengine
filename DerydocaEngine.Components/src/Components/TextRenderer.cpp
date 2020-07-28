#include "EngineComponentsPch.h"
#include "TextRenderer.h"

#include "Rendering\Material.h"
#include "Utilities\TexturePackerImage.h"

namespace DerydocaEngine::Components
{

	const int CARRIAGE_RETURN_CHAR = 13;
	const int DEL_CHAR = 127;

	TextRenderer::TextRenderer() :
		m_Color(1.0f, 1.0f, 1.0f, 1.0f),
		m_Bounds(),
		m_OverflowWrap(OverflowWrap::Normal),
		m_HorizontalAlign(TextAlign::Start),
		m_VerticalAlign(TextAlign::Start),
		m_Text(),
		m_Shader(),
		m_FontFace(),
		m_Lines(),
		m_FilteredText(nullptr),
		m_TextDirty(true),
		m_Material()
	{
	}

	TextRenderer::~TextRenderer()
	{
	}

	void TextRenderer::postInit()
	{
		if (m_FontFace)
		{
			auto ff = std::static_pointer_cast<UI::FontFace>(m_FontFace.GetSmartPointer()->getResourceObjectPointer());
			auto fontTexture = ff->getTexture();
			getMaterial()->setTexture("CharacterSheet", fontTexture);
			markComponentAsDirty(DIRTY_COMPONENTS_ON_INDICES_CHANGED);
			markComponentAsDirty(DIRTY_COMPONENTS_ON_TEXT_CHANGE);
		}
	}

	SERIALIZE_FUNC_LOAD(archive, TextRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::RendererComponent),
			SERIALIZE(m_Color),
			SERIALIZE(m_Bounds),
			SERIALIZE(m_OverflowWrap),
			SERIALIZE(m_HorizontalAlign),
			SERIALIZE(m_VerticalAlign),
			SERIALIZE(m_Text),
			SERIALIZE(m_Shader),
			SERIALIZE(m_FontFace)
		);

		auto material = std::make_shared<Rendering::Material>();
		material->setShader(m_Shader);
		setMaterial(material);

		setText(m_Text);

		markComponentAsDirty(Rendering::MeshComponents::All);
		updateMesh();
	}

	SERIALIZE_FUNC_SAVE(archive, TextRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::RendererComponent),
			SERIALIZE(m_Color),
			SERIALIZE(m_Bounds),
			SERIALIZE(m_OverflowWrap),
			SERIALIZE(m_HorizontalAlign),
			SERIALIZE(m_VerticalAlign),
			SERIALIZE(m_Text),
			SERIALIZE(m_Shader),
			SERIALIZE(m_FontFace)
		);
	}

	void TextRenderer::setText(const std::string& text)
	{
		m_Text = text;
		auto ff = std::static_pointer_cast<UI::FontFace>(m_FontFace.GetSmartPointer()->getResourceObjectPointer());
		m_Lines = processTextToLines(m_Text, m_OverflowWrap, ff, m_Bounds.x, m_FilteredText);
		int newVertCount = generateNumVertices();
		int currentVertCount = getNumVertices();
		if (newVertCount != currentVertCount)
		{
			markComponentAsDirty(DIRTY_COMPONENTS_ON_INDICES_CHANGED);
		}
		markComponentAsDirty(DIRTY_COMPONENTS_ON_TEXT_CHANGE);
	}

	std::vector<glm::vec3> TextRenderer::generateVertices()
	{
		// Store the current pen position
		float penX = 0.0f;
		float penY = 0.0f;

		// Calculate the information we need for vertical alignment
		float lineHeight = 0.0f;
		auto ff = std::static_pointer_cast<UI::FontFace>(m_FontFace.GetSmartPointer()->getResourceObjectPointer());
		calculateVerticalAlignmentProperties(m_VerticalAlign, (int)m_Lines.size(), m_Bounds.y, ff->getLineHeight(), &penY, &lineHeight);

		// Keep reference to the quad that we are currently building
		int charQuadIndex = 0;

		std::vector<glm::vec3> vertices;
		vertices.reserve(generateNumVertices());

		// Iterate through all lines of the text
		for (size_t lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++)
		{
			LineProperties* lineProperties = m_Lines[lineIndex];

			// Calculate the information we need for horizontal alignment
			float extraCharAdvance = 0.0f;
			calculateHorizontalAlignmentProperties(m_HorizontalAlign, m_Bounds.x, lineProperties->getWidth(), lineProperties->getEnd() - lineProperties->getStart(), lineProperties->getStartAdjust(), &penX, &extraCharAdvance);

			// Adjust the x position based on the line's start adjustment
			penX += lineProperties->getStartAdjust();

			// Iterate through each character in the line
			for (int charIndex = lineProperties->getStart(); charIndex < lineProperties->getEnd(); charIndex++)
			{
				// Get the character image information
				auto ff = std::static_pointer_cast<UI::FontFace>(m_FontFace.GetSmartPointer()->getResourceObjectPointer());
				Utilities::TexturePackerImage img = ff->getCharData(m_FilteredText[charIndex]);

				// Set the vertex positions
				float2 bearing = img.bearing;
				float2 worldSize = img.size;
				float charXMin = penX + bearing.x;
				float charXMax = penX + bearing.x + worldSize.x;
				float charYMax = penY - worldSize.y + bearing.y;
				float charYMin = penY + bearing.y;
				vertices.push_back(glm::vec3(charXMin, charYMin, 0));
				vertices.push_back(glm::vec3(charXMin, charYMax, 0));
				vertices.push_back(glm::vec3(charXMax, charYMax, 0));
				vertices.push_back(glm::vec3(charXMax, charYMin, 0));

				// Advance the pen position
				float2 advance = img.advance;
				penX += advance.x + extraCharAdvance;
				penY += advance.y;

				// Continue on to the next char
				charQuadIndex++;
			}

			// Move the pen line down an entire line height
			penY -= lineHeight;
		}

		return vertices;
	}

	std::vector<glm::vec2> TextRenderer::generateTexCoords()
	{
		// Keep reference to the quad that we are currently building
		int charQuadIndex = 0;

		std::vector<glm::vec2> texCoords;
		texCoords.reserve(generateNumVertices());

		// Iterate through all lines of the text
		for (size_t lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++)
		{
			LineProperties* lineProperties = m_Lines[lineIndex];

			// Iterate through each character in the line
			for (int charIndex = lineProperties->getStart(); charIndex < lineProperties->getEnd(); charIndex++)
			{
				// Get the character image information
				auto ff = std::static_pointer_cast<UI::FontFace>(m_FontFace.GetSmartPointer()->getResourceObjectPointer());
				Utilities::TexturePackerImage img = ff->getCharData(m_FilteredText[charIndex]);

				// Set the UV positions
				Rect rect = img.imageRect;
				texCoords.push_back(glm::vec2(rect.getX(), rect.getY()));
				texCoords.push_back(glm::vec2(rect.getX(), rect.getDY()));
				texCoords.push_back(glm::vec2(rect.getDX(), rect.getDY()));
				texCoords.push_back(glm::vec2(rect.getDX(), rect.getY()));

				// Continue on to the next char
				charQuadIndex++;
			}
		}

		return texCoords;
	}

	std::vector<Color> TextRenderer::generateVertexColors()
	{
		// Keep reference to the quad that we are currently building
		int charQuadIndex = 0;

		std::vector<Color> vertexColors;
		vertexColors.reserve(generateNumVertices());

		// Iterate through all lines of the text
		for (size_t lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++)
		{
			LineProperties* lineProperties = m_Lines[lineIndex];

			// Iterate through each character in the line
			for (int charIndex = lineProperties->getStart(); charIndex < lineProperties->getEnd(); charIndex++)
			{
				// Set the vertex colors
				vertexColors.push_back(m_Color);
				vertexColors.push_back(m_Color);
				vertexColors.push_back(m_Color);
				vertexColors.push_back(m_Color);

				// Continue on to the next char
				charQuadIndex++;
			}
		}

		return vertexColors;
	}

	std::vector<unsigned int> TextRenderer::generateTriangleIndices()
	{
		// Keep reference to the quad that we are currently building
		int charQuadIndex = 0;

		std::vector<unsigned int> triangleIndices;
		triangleIndices.reserve(generateNumIndices());

		// Iterate through all lines of the text
		for (size_t lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++)
		{
			LineProperties* lineProperties = m_Lines[lineIndex];

			// Iterate through each character in the line
			for (int charIndex = lineProperties->getStart(); charIndex < lineProperties->getEnd(); charIndex++)
			{
				// Get the character image information
				auto ff = std::static_pointer_cast<UI::FontFace>(m_FontFace.GetSmartPointer()->getResourceObjectPointer());
				Utilities::TexturePackerImage img = ff->getCharData(m_FilteredText[charIndex]);

				// Set the indices to draw the two triangles
				triangleIndices.push_back(charQuadIndex * 4 + 0);
				triangleIndices.push_back(charQuadIndex * 4 + 1);
				triangleIndices.push_back(charQuadIndex * 4 + 2);
				triangleIndices.push_back(charQuadIndex * 4 + 0);
				triangleIndices.push_back(charQuadIndex * 4 + 2);
				triangleIndices.push_back(charQuadIndex * 4 + 3);

				// Continue on to the next char
				charQuadIndex++;
			}
		}

		return triangleIndices;
	}

	void TextRenderer::calculateVerticalAlignmentProperties(TextAlign const& alignment, int const& numberOfLines, float const& verticalBoundSize, float const& fontLineHeight, float * const& penY, float * const& newLineHeight)
	{
		switch (alignment)
		{
		case Start:
			// Calculate top alignment
			*newLineHeight = fontLineHeight;
			*penY = 0.0f;
			break;
		case Center:
			// Calculate center alignment
			*newLineHeight = fontLineHeight;
			*penY = -(verticalBoundSize - (numberOfLines * *newLineHeight) / 2);
			break;
		case End:
			// Calculate bottom alignment
			*newLineHeight = fontLineHeight;
			*penY = -(verticalBoundSize - (numberOfLines * *newLineHeight));
			break;
		case Justify:
			// Calculate justified alignment
			*newLineHeight = numberOfLines ? (verticalBoundSize - fontLineHeight) / numberOfLines : fontLineHeight;
			*penY = 0.0f;
			break;
		default:
			*newLineHeight = fontLineHeight;
			*penY = 0.0f;
		}
	}

	void TextRenderer::calculateHorizontalAlignmentProperties(TextAlign const& alignment, float const& horizontalBoundSize, float const& lineWidth, int const& numChars, float const& lineStartAdjust, float * const& penX, float* const& extraCharAdvance)
	{
		switch (alignment)
		{
		case Center:
			// Calculate left alignment
			*penX = (horizontalBoundSize - lineWidth - lineStartAdjust) / 2.0f;
			break;
		case End:
			// Calculate right alignment
			*penX = horizontalBoundSize - lineWidth - lineStartAdjust;
			break;
		case Justify:
			// Calculate justified alignment
			*penX = 0.0f;
			if (horizontalBoundSize != 0 && numChars != 0)
			{
				*extraCharAdvance = (horizontalBoundSize - lineWidth) / numChars;
			}
			break;
		default:
			*penX = 0.0f;
			break;
		}
	}

	std::vector<LineProperties*> TextRenderer::processTextToLines(
		std::string const& text,
		OverflowWrap const& overflowWrap,
		std::shared_ptr<UI::FontFace> fontFace,
		float const& horizontalBoundSize,
		char*& filteredText
	)
	{
		// Create an array to store the filtered text. This will never excede the length of the source text.
		filteredText = new char[text.length()];

		// Indices of where the read/write head is of the current character being processed
		int filteredTextIndex = 0;
		int sourceTextIndex = 0;

		// Toggle boolean for breaking out of the process loop
		bool continueLookingForLines = true;

		// Information about the current line that will later be stored in the line's properties
		float currentLineWidth = 0.0f;
		float currentLineStartAdjust = 0.0f;

		// Remember if the current line started via a forced line break
		bool isLineAForcedBreak = false;

		// Information about the last breakable line position
		float lastBreakableWidth = 0.0f;
		int lastBreakableSourceIndex = 0;
		int lastBreakableFilteredIndex = 0;

		// Used for Normal overflow wrap when a line exceedes the bounds
		bool lookingForNextLineBreak = false;

		// Indices of where the next line of text will begin
		int newLineSourceTextIndex = 0;
		int newLineFilteredTextIndex = 0;

		// Keep information about all of the line properties
		std::vector<LineProperties*> allLineProperties;

		// Start the first line at position zero
		LineProperties* lineProperties = new LineProperties(0);

		while (continueLookingForLines)
		{
			int filteredTextEndIndex = 0;
			bool addCharacterToFilteredText = true;
			char c = text[sourceTextIndex];

			// If the current character is a carriage return, end the line
			if (c == CARRIAGE_RETURN_CHAR)
			{
				filteredTextEndIndex = filteredTextIndex;
				isLineAForcedBreak = false;
				addCharacterToFilteredText = false;
				newLineSourceTextIndex = sourceTextIndex + 1;
				newLineFilteredTextIndex = filteredTextIndex;
			}
			// If the current character is a null-terminator, end the line and stop the process
			else if (c == 0)
			{
				filteredTextEndIndex = filteredTextIndex;
				isLineAForcedBreak = true;
				addCharacterToFilteredText = false;
				continueLookingForLines = false;
				newLineFilteredTextIndex = filteredTextIndex;
				newLineSourceTextIndex = (int)text.size();
			}

			// If we are still adding this character to the filtered text
			if (addCharacterToFilteredText)
			{
				// Get the character image information
				Utilities::TexturePackerImage img = fontFace->getCharData(c);
				bool isWhitespace = img.size.x == 0.0f;
				bool isBreakable = c == ' ' || c == '-';

				// If the character can break the line, store information about this position
				if (isBreakable)
				{
					lastBreakableFilteredIndex = filteredTextIndex;
					lastBreakableSourceIndex = sourceTextIndex;
					lastBreakableWidth = currentLineWidth;

					// If we are looking for a line break, end the line
					if (lookingForNextLineBreak)
					{
						filteredTextEndIndex = filteredTextIndex + 1;
						isLineAForcedBreak = true;
						newLineSourceTextIndex = sourceTextIndex + 1;
						newLineFilteredTextIndex = filteredTextIndex + 1;
					}
				}

				float2 advance = img.advance;

				// If this line of text starts with whitespace and this line was started by a forced line break
				if (isLineAForcedBreak && isWhitespace && currentLineWidth == 0.0f)
				{
					currentLineStartAdjust -= advance.x;
				}

				// Check to see if this next character will break the horizontal boundary
				if (!lookingForNextLineBreak && horizontalBoundSize > 0 && (currentLineWidth + advance.x + currentLineStartAdjust) > horizontalBoundSize)
				{
					// If the overflow wrap is set to break on all characters, end the line here
					if (overflowWrap == OverflowWrap::BreakAll)
					{
						filteredTextEndIndex = filteredTextIndex;
						isLineAForcedBreak = true;
						newLineSourceTextIndex = sourceTextIndex;
						newLineFilteredTextIndex = filteredTextIndex;
					}
					// Otherwise...
					else
					{
						// If the last breakable index is not at the start of this line, end it there
						if (lastBreakableFilteredIndex > lineProperties->getStart())
						{
							filteredTextEndIndex = lastBreakableFilteredIndex;
							isLineAForcedBreak = true;
							currentLineWidth = lastBreakableWidth;
							newLineSourceTextIndex = lastBreakableSourceIndex;
							newLineFilteredTextIndex = lastBreakableFilteredIndex;
						}
						// Otherwise, we need to find the next available breakable spot
						// If the overflow is set to Normal, ignore the bounds check until we find a breakable character
						else if (overflowWrap == OverflowWrap::Normal)
						{
							lookingForNextLineBreak = true;
							currentLineWidth += advance.x;
						}
						// Otherwise, break the word at the current index
						else
						{
							filteredTextEndIndex = filteredTextIndex;
							isLineAForcedBreak = true;
							newLineSourceTextIndex = sourceTextIndex;
							newLineFilteredTextIndex = filteredTextIndex;
						}
					}
				}
				// If this character does not break the horizontal boundary, increase the line's width
				else
				{
					currentLineWidth += advance.x;
				}

				// Remove the trailing whitespace if there is any
				if (filteredTextEndIndex > 0 && isWhitespace)
				{
					currentLineWidth -= advance.x;
				}
			}

			// Add the character to the filtered text if we are still sure we want to add it
			if (addCharacterToFilteredText)
			{
				filteredText[filteredTextIndex++] = c;
			}

			// If an end index has been defined for this line, lets end it
			if (filteredTextEndIndex > 0)
			{
				// Store the line properties
				lineProperties->setEnd(filteredTextEndIndex);
				lineProperties->setWidth(currentLineWidth);
				lineProperties->setStartAdjust(currentLineStartAdjust);

				// Add this line to the line collection
				allLineProperties.push_back(lineProperties);

				// Create a new line starting at the end of the last line
				lineProperties = new LineProperties(filteredTextEndIndex);

				// Reset temporary line variables
				currentLineWidth = 0.0f;
				currentLineStartAdjust = 0.0f;
				filteredTextEndIndex = 0;
				lookingForNextLineBreak = false;

				// Reset the read/write indices to where the new line is starting at
				sourceTextIndex = newLineSourceTextIndex;
				filteredTextIndex = newLineFilteredTextIndex;
			}
			// If we are not adding the character to the filtered text, increment the source text index
			else
			{
				sourceTextIndex++;
			}
		}

		// Serve up all of the line information we found
		return allLineProperties;
	}

}
