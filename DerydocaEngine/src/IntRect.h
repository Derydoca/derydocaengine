#pragma once

namespace DerydocaEngine
{

	struct IntRect
	{
	public:
		IntRect() :
			x(0),
			y(0),
			dx(1),
			dy(1)
		{
		}

		IntRect(int const& x, int const& y, int const& dx, int const& dy) :
			x(x),
			y(y),
			dx(dx),
			dy(dy)
		{
		}

		inline static bool IsRectOverlapping(const IntRect& r1, const IntRect& r2)
		{
			return r1.dy > r2.y&& r1.y < r2.dy && r1.x < r2.dx && r1.dx > r2.x;
		}

		int x, y, dx, dy;
	};

}
