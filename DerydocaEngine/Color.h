#pragma once

namespace DerydocaEngine
{

	struct Color
	{
	public:
		Color() : r(0), g(0), b(0), a(1) {}
		Color(float const& r, float const& g, float const& b) : r(r), g(g), b(b), a(1) {}
		Color(float const& r, float const& g, float const& b, float const& a) : r(r), g(g), b(b), a(a) {}

		float r, g, b, a;

		bool operator==(Color const& rhs) const
		{
			return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
		}

		bool operator!=(Color const& rhs) const
		{
			return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.b;
		}
	};

}
