#pragma once

namespace DerydocaEngine
{

	struct Rectangle
	{
	public:
		Rectangle()
		{
			m_x = 0.0f;
			m_y = 0.0f;
			m_width = 1.0f;
			m_height = 1.0f;
		}

		Rectangle(float const& x, float const& y, float const& width, float const& height) :
			m_x(x),
			m_y(y),
			m_width(width),
			m_height(height)
		{
		}

		float getX() const { return m_x; }
		float getY() const { return m_y; }
		float getWidth() const { return m_width; }
		float getHeight() const { return m_height; }
		void setX(float const& x) { m_x = x; }
		void setY(float const& y) { m_y = y; }
		void setWidth(float const& w) { m_width = w; }
		void setHeight(float const& h) { m_height = h; }
	private:
		float m_x, m_y, m_width, m_height;
	};

}
