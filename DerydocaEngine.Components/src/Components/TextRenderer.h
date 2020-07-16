#pragma once
#include <string>
#include "UI\FontFace.h"
#include "RendererComponent.h"
#include "Resources\ShaderResource.h"
#include "Resources\RasterFontResource.h"

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
		SERIALIZE_FUNC_DEFINITIONS;

		TextRenderer();
		~TextRenderer();

		virtual void postInit();

		void setText(const std::string& text);
		std::string getText() { return m_Text; }
		Color getColor() const { return m_Color; }
		void setColor(Color const& color)
		{
			if (m_Color == color)
			{
				return;
			}

			m_Color = color;
			markComponentAsDirty(Rendering::MeshComponents::Colors);
		}

		virtual std::vector<glm::vec3> generateVertices();
		virtual std::vector<glm::vec2> generateTexCoords();
		virtual std::vector<Color> generateVertexColors();
		virtual std::vector<unsigned int> generateTriangleIndices();
		int generateNumVertices()
		{
			if (m_Lines.size() == 0)
			{
				return 0;
			}
			int charCount = m_Lines.back()->getEnd();
			return charCount * 4;
		}
		virtual unsigned int generateNumIndices()
		{
			if (m_Lines.size() == 0)
			{
				return 0;
			}
			int charCount = m_Lines.back()->getEnd();
			return charCount * 6;
		}

	private:
		const Rendering::MeshComponents DIRTY_COMPONENTS_ON_TEXT_CHANGE = (Rendering::MeshComponents)(Rendering::MeshComponents::Positions | Rendering::MeshComponents::TexCoords);
		const Rendering::MeshComponents DIRTY_COMPONENTS_ON_INDICES_CHANGED = (Rendering::MeshComponents)(Rendering::MeshComponents::Colors | Rendering::MeshComponents::Indices);

		Color m_Color;
		glm::vec2 m_Bounds;
		OverflowWrap m_OverflowWrap;
		TextAlign m_HorizontalAlign;
		TextAlign m_VerticalAlign;
		std::string m_Text;
		ResourceRef<Resources::ShaderResource> m_Shader;
		ResourceRef<Resources::RasterFontResource> m_FontFace;

		std::vector<LineProperties*> m_Lines;
		char* m_FilteredText;
		bool m_TextDirty;
		std::shared_ptr<Rendering::Material> m_Material;

		static void calculateVerticalAlignmentProperties(TextAlign const& alignment, int const& numberOfLines, float const& verticalBoundSize, float const& fontLineHeight, float* const& penY, float* const& newLineHeight);
		static void calculateHorizontalAlignmentProperties(TextAlign const& alignment, float const& horizontalBoundSize, float const& lineWidth, int const& numChars, float const& lineStartAdjust, float* const& penX, float* const& extraCharAdvance);
		static std::vector<LineProperties*> processTextToLines(
			std::string const& text,
			OverflowWrap const& overflowWrap,
			std::shared_ptr<UI::FontFace> fontFace,
			float const& horizontalBoundSize,
			char*& filteredText
		);
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::TextRenderer, 0);
