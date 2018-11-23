#pragma once
#include <glm/glm.hpp>
#include "Rect.h"

namespace DerydocaEngine::Utilities
{

	struct TexturePackerImage
	{
	public:
		TexturePackerImage() {};
		TexturePackerImage(
			unsigned long const& id,
			int const& width,
			int const& height,
			int const& channels,
			float const& sizeX,
			float const& sizeY,
			float const& bearingX,
			float const& bearingY,
			float const& advanceX,
			float const& advanceY);
		~TexturePackerImage();

		unsigned long getID() const { return m_id; }
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		int getChannels() const { return m_channels; }
		float getSizeX() const { return m_sizeX; }
		float getSizeY() const { return m_sizeY; }
		float getBearingX() const { return m_bearingX; }
		float getBearingY() const { return m_bearingY; }
		float getAdvanceX() const { return m_advanceX; }
		float getAdvanceY() const { return m_advanceY; }
		Rect getTexSheetPosition() { return m_texSheetPosition; }

		void setTextureSheetRectangle(float const& x, float const& y, float const& dx, float const& dy);

	private:
		unsigned long m_id;
		int m_width;
		int m_height;
		int m_channels;
		float m_sizeX;
		float m_sizeY;
		float m_advanceX;
		float m_advanceY;
		float m_bearingX;
		float m_bearingY;
		Rect m_texSheetPosition;
	};

}
