#pragma once

#include "Math\TypeAliases.h"
#include "Serialization\Serialization.h"

namespace glm
{

	template<class Archive>
	void serialize(Archive& archive, glm::ivec2& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec2& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec3& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y),
			SERIALIZE_NAMED("z", m.z)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec4& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y),
			SERIALIZE_NAMED("z", m.z),
			SERIALIZE_NAMED("w", m.w)
		);
	}

}