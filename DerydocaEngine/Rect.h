#pragma once

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

	Rect(float x, float y, float dx, float dy) :
		m_x(x),
		m_y(y),
		m_dx(dx),
		m_dy(dy)
	{
	}

	float getX() { return m_x; }
	float getY() { return m_y; }
	float getDX() { return m_dx; }
	float getDY() { return m_dy; }
	void set(float x, float y, float dx, float dy)
	{
		m_x = x;
		m_y = y;
		m_dx = dx;
		m_dy = dy;
	}
	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }
	void setDX(float w) { m_dx = w; }
	void setDY(float h) { m_dy = h; }
private:
	float m_x, m_y, m_dx, m_dy;
};