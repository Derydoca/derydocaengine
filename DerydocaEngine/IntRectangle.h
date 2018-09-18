#pragma once

namespace DerydocaEngine
{

	struct IntRectangle
	{
	public:
		IntRectangle()
		{
			m_x = 0;
			m_y = 0;
			m_width = 1;
			m_height = 1;
		}

		IntRectangle(int const& x, int const& y, int const& width, int const& height) :
			m_x(x),
			m_y(y),
			m_width(width),
			m_height(height)
		{
		}

		int getX() { return m_x; }
		int getY() { return m_y; }
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		void setX(int const& x) { m_x = x; }
		void setY(int const& y) { m_y = y; }
		void setWidth(int const& w) { m_width = w; }
		void setHeight(int const& h) { m_height = h; }

		static bool IsRectOverlapping(IntRectangle & r1, IntRectangle & r2);
		static bool IsPointInRect(IntRectangle & rect, int const& x, int const& y);
	private:
		int m_x, m_y, m_width, m_height;
	};

}
