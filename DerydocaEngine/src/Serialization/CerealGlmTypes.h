#pragma once

#include "Math\TypeAliases.h"
#include "Serialization\Serialization.h"

namespace glm
{

	template<class Archive>
	void save(Archive& archive, glm::fquat const & q)
	{
		archive(
			SERIALIZE_NAMED("x", q.x),
			SERIALIZE_NAMED("y", q.y),
			SERIALIZE_NAMED("z", q.z),
			SERIALIZE_NAMED("w", q.w)
		);
	}
	template<class Archive>
	void load(Archive& archive, glm::fquat& q)
	{
		archive(
			SERIALIZE_NAMED("x", q.x),
			SERIALIZE_NAMED("y", q.y),
			SERIALIZE_NAMED("z", q.z),
			SERIALIZE_NAMED("w", q.w)
		);
	}

	template<class Archive>
	void load(Archive& archive, glm::ivec2& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y)
		);
	}
	template<class Archive>
	void save(Archive& archive, glm::ivec2 const & m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y)
		);
	}

	template<class Archive>
	void load(Archive& archive, glm::vec2& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y)
		);
	}
	template<class Archive>
	void save(Archive& archive, glm::vec2 const & m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y)
		);
	}

	template<class Archive>
	void load(Archive& archive, glm::vec3& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y),
			SERIALIZE_NAMED("z", m.z)
		);
	}
	template<class Archive>
	void save(Archive& archive, glm::vec3 const & m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y),
			SERIALIZE_NAMED("z", m.z)
		);
	}

	template<class Archive>
	void load(Archive& archive, glm::vec4& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y),
			SERIALIZE_NAMED("z", m.z),
			SERIALIZE_NAMED("w", m.w)
		);
	}
	template<class Archive>
	void save(Archive& archive, glm::vec4 const& m)
	{
		archive(
			SERIALIZE_NAMED("x", m.x),
			SERIALIZE_NAMED("y", m.y),
			SERIALIZE_NAMED("z", m.z),
			SERIALIZE_NAMED("w", m.w)
		);
	}

	template<class Archive>
	void load(Archive& archive, glm::mat3& m)
	{
		archive(
			SERIALIZE_NAMED("m_00", m[0]),
			SERIALIZE_NAMED("m_01", m[1]),
			SERIALIZE_NAMED("m_02", m[2]),
			SERIALIZE_NAMED("m_10", m[3]),
			SERIALIZE_NAMED("m_11", m[4]),
			SERIALIZE_NAMED("m_12", m[5]),
			SERIALIZE_NAMED("m_20", m[6]),
			SERIALIZE_NAMED("m_21", m[7]),
			SERIALIZE_NAMED("m_22", m[8])
		);
	}
	template<class Archive>
	void save(Archive& archive, glm::mat3 const& m)
	{
		archive(
			SERIALIZE_NAMED("m_00", m[0]),
			SERIALIZE_NAMED("m_01", m[1]),
			SERIALIZE_NAMED("m_02", m[2]),
			SERIALIZE_NAMED("m_10", m[3]),
			SERIALIZE_NAMED("m_11", m[4]),
			SERIALIZE_NAMED("m_12", m[5]),
			SERIALIZE_NAMED("m_20", m[6]),
			SERIALIZE_NAMED("m_21", m[7]),
			SERIALIZE_NAMED("m_22", m[8])
		);
	}

	template<class Archive>
	void load(Archive& archive, glm::mat4& m)
	{
		archive(
			SERIALIZE_NAMED("m_00", m[0]),
			SERIALIZE_NAMED("m_01", m[1]),
			SERIALIZE_NAMED("m_02", m[2]),
			SERIALIZE_NAMED("m_03", m[3]),
			SERIALIZE_NAMED("m_10", m[4]),
			SERIALIZE_NAMED("m_11", m[5]),
			SERIALIZE_NAMED("m_12", m[6]),
			SERIALIZE_NAMED("m_13", m[7]),
			SERIALIZE_NAMED("m_20", m[8]),
			SERIALIZE_NAMED("m_21", m[9]),
			SERIALIZE_NAMED("m_22", m[10]),
			SERIALIZE_NAMED("m_23", m[11]),
			SERIALIZE_NAMED("m_30", m[12]),
			SERIALIZE_NAMED("m_31", m[13]),
			SERIALIZE_NAMED("m_32", m[14]),
			SERIALIZE_NAMED("m_33", m[15])
		);
	}
	template<class Archive>
	void save(Archive& archive, glm::mat4 const& m)
	{
		archive(
			SERIALIZE_NAMED("m_00", m[0]),
			SERIALIZE_NAMED("m_01", m[1]),
			SERIALIZE_NAMED("m_02", m[2]),
			SERIALIZE_NAMED("m_03", m[3]),
			SERIALIZE_NAMED("m_10", m[4]),
			SERIALIZE_NAMED("m_11", m[5]),
			SERIALIZE_NAMED("m_12", m[6]),
			SERIALIZE_NAMED("m_13", m[7]),
			SERIALIZE_NAMED("m_20", m[8]),
			SERIALIZE_NAMED("m_21", m[9]),
			SERIALIZE_NAMED("m_22", m[10]),
			SERIALIZE_NAMED("m_23", m[11]),
			SERIALIZE_NAMED("m_30", m[12]),
			SERIALIZE_NAMED("m_31", m[13]),
			SERIALIZE_NAMED("m_32", m[14]),
			SERIALIZE_NAMED("m_33", m[15])
		);
	}

}