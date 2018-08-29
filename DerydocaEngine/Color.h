#pragma once

struct Color
{
public:
	Color() : r(0), g(0), b(0), a(1) {}
	Color(float r, float g, float b) : r(r), g(g), b(b), a(1) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	float r, g, b, a;

	bool operator==(const Color& rhs) const
	{
		return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
	}

	bool operator!=(const Color& rhs) const
	{
		return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.b;
	}
};