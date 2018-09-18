#pragma once

namespace DerydocaEngine
{

	struct Rect
	{
	public:
		Rect()
		{
			m_x = 0.0f;
			m_y = 0.0f;
			m_dx = 1.0f;
			m_dy = 1.0f;
		}

		Rect(float const& x, float const& y, float const& dx, float const& dy) :
			m_x(x),
			m_y(y),
			m_dx(dx),
			m_dy(dy)
		{
		}

		float getX() const { return m_x; }
		float getY() const { return m_y; }
		float getDX() const { return m_dx; }
		float getDY() const { return m_dy; }
		void set(float const& x, float const& y, float const& dx, float const& dy)
		{
			m_x = x;
			m_y = y;
			m_dx = dx;
			m_dy = dy;
		}
		void setX(float const& x) { m_x = x; }
		void setY(float const& y) { m_y = y; }
		void setDX(float const& w) { m_dx = w; }
		void setDY(float const& h) { m_dy = h; }
	private:
		float m_x, m_y, m_dx, m_dy;
	};

}
