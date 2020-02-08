#pragma once

namespace DerydocaEngine
{
	struct int2
	{
		int x;
		int y;

		int2()
		{
			x = 0;
			y = 0;
		}

		int2(int _x, int _y)
		{
			x = _x;
			y = _y;
		}

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				SERIALIZE(x),
				SERIALIZE(y)
			);
		}
	};
}