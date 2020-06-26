#pragma once

namespace DerydocaEngine
{

	struct Color
	{
	public:
		Color() : r(0), g(0), b(0), a(1) {}
		Color(float const& r, float const& g, float const& b) : r(r), g(g), b(b), a(1) {}
		Color(float const& r, float const& g, float const& b, float const& a) : r(r), g(g), b(b), a(a) {}
		Color(glm::vec3 color) : r(color[0]), g(color[1]), b(color[2]), a(1) {}
		Color(glm::vec4 color) : r(color[0]), g(color[1]), b(color[2]), a(color[3]) {}

		float r, g, b, a;

		bool operator==(Color const& rhs) const
		{
			return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
		}

		bool operator!=(Color const& rhs) const
		{
			return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.b;
		}

		glm::vec4 toVec4()
		{
			return glm::vec4(r, g, b, a);
		}

		SERIALIZE_FUNC(
			SERIALIZE_NAMED("r", r),
			SERIALIZE_NAMED("g", g),
			SERIALIZE_NAMED("b", b),
			SERIALIZE_NAMED("a", a)
		);
	};

}
