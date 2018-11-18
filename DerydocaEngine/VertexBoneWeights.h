#pragma once

#include <array>

namespace DerydocaEngine::Animation {

	static const int MAX_BONES = 4;

	struct VertexBoneWeights {
		std::array<unsigned int, MAX_BONES> boneIds;
		std::array<float, MAX_BONES> weights;

		VertexBoneWeights()
		{
			boneIds.fill(0);
			weights.fill(0.0f);
		}
		
	};

}
