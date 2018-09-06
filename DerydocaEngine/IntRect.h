#pragma once

struct IntRect
{
public:
	IntRect()
	{
		m_x = 0;
		m_y = 0;
		m_dx = 1;
		m_dy = 1;
	}

	IntRect(int const& x, int const& y, int const& dx, int const& dy) :
		m_x(x),
		m_y(y),
		m_dx(dx),
		m_dy(dy)
	{
	}

	int getX() { return m_x; }
	int getY() { return m_y; }
	int getDx() { return m_dx; }
	int getDy() { return m_dy; }
	void setX(int const& x) { m_x = x; }
	void setY(int const& y) { m_y = y; }
	void setDx(int const& dx) { m_dx = dx; }
	void setDy(int const& dy) { m_dy = dy; }

	static bool IsRectOverlapping(IntRect & r1, IntRect & r2);
private:
	int m_x, m_y, m_dx, m_dy;
};