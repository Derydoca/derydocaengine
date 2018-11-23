#pragma once

namespace DerydocaEngine::Animation {

	template <typename T>
	struct AnimationKey
	{
		AnimationKey() :
			time(0)
			value()
		{
		}

		AnimationKey(double time, T value) :
			time(time),
			value(value)
		{
		}

		double time;
		T value;
	};

}
