#pragma once
#include <string>
#include "FontFace.h"
#include "RendererComponent.h"

namespace DerydocaEngine::Rendering {
	class Material;
}

namespace DerydocaEngine::Components
{

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
		void setStart(int const& start) { m_start = start; }
		void setEnd(int const& end) { m_end = end; }
		void setWidth(float const& width) { m_width = width; }
		void setStartAdjust(float const& startAdjust) { m_startAdjust = startAdjust; }
	private:
		int m_start;
		int m_end;
		float m_width;
		float m_startAdjust;
	};

	class TextRenderer : public RendererComponent, Components::SelfRegister<TextRenderer>
	{
	public:
		GENINSTANCE(TextRenderer);

		virtual void postInit();
		virtual void deserialize(YAML::Node const& compNode);

		void setText(std::string const& text)
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
		std::string getText() { return m_text; }
		void setColor(Color const& color)
		{
			if (m_textColor == color)
			{
				return;
			}

			m_textColor = color;
			markComponentAsDirty(Rendering::MeshComponents::Colors);
		}

		virtual std::vector<glm::vec3> generateVertices();
		virtual std::vector<glm::vec2> generateTexCoords();
		virtual std::vector<Color> generateVertexColors();
		virtual std::vector<unsigned int> generateTriangleIndices();
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
		const Rendering::MeshComponents DIRTY_COMPONENTS_ON_TEXT_CHANGE = (Rendering::MeshComponents)(Rendering::MeshComponents::Positions | Rendering::MeshComponents::TexCoords);
		const Rendering::MeshComponents DIRTY_COMPONENTS_ON_INDICES_CHANGED = (Rendering::MeshComponents)(Rendering::MeshComponents::Colors | Rendering::MeshComponents::Indices);

		Rendering::Material* m_material;
		UI::FontFace* m_fontFace;
		std::string m_text = "Text";
		glm::vec2 m_bounds;
		Color m_textColor;
		OverflowWrap m_overflowWrap;
		TextAlign m_horizontalAlign;
		TextAlign m_verticalAlign;
		std::vector<LineProperties*> m_lines;
		char* m_filteredText;
		bool m_textDirty = true;

		static void calculateVerticalAlignmentProperties(TextAlign const& alignment, int const& numberOfLines, float const& verticalBoundSize, float const& fontLineHeight, float* const& penY, float* const& newLineHeight);
		static void calculateHorizontalAlignmentProperties(TextAlign const& alignment, float const& horizontalBoundSize, float const& lineWidth, int const& numChars, float const& lineStartAdjust, float* const& penX, float* const& extraCharAdvance);
		static std::vector<LineProperties*> processTextToLines(std::string const& text, OverflowWrap const& overflowWrap, UI::FontFace* const& fontFace, float const& horizontalBoundSize, char*& filteredText);
	};

}
