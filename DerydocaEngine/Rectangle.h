#pragma once
#include <assert.h>

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

	Rectangle(float x, float y, float width, float height) :
		m_x(x),
		m_y(y),
		m_width(width),
		m_height(height)
	{
	}

	float getX() { return m_x; }
	float getY() { return m_y; }
	float getWidth() { return m_width; }
	float getHeight() { return m_height; }
	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }
	void setWidth(float w) { m_width = w; }
	void setHeight(float h) { m_height = h; }
private:
	float m_x, m_y, m_width, m_height;
};