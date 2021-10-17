#pragma once
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "Resources\TextureResource.h"

namespace DerydocaEngine::AssetData
{
	template<class T>
	struct MaterialPropertyData
	{
		MaterialPropertyData() :
			Name(),
			Value()
		{
		}

		MaterialPropertyData(const std::string& name, T value) :
			Name(name),
			Value(value)
		{
		}

		std::string Name;
		T Value;

		SERIALIZE_FUNC(
			SERIALIZE(Name),
			SERIALIZE(Value)
		);
	};

	struct SubroutineMap
	{
		SubroutineMap() :
			Program(),
			Value()
		{
		}

		SubroutineMap(unsigned int program, unsigned int value) :
			Program(program),
			Value(value)
		{
		}

		unsigned int Program;
		unsigned int Value;

		SERIALIZE_FUNC(
			SERIALIZE(Program),
			SERIALIZE(Value)
		);
	};

	struct MaterialData
	{
		MaterialData()
		{
		}

		uuids::uuid ShaderId;
		std::vector<MaterialPropertyData<ResourceRef<Resources::TextureResource>>> Textures;
		std::vector<MaterialPropertyData<bool>> Bools;
		std::vector<MaterialPropertyData<int>> Ints;
		std::vector<MaterialPropertyData<float>> Floats;
		std::vector<MaterialPropertyData<std::vector<float>>> FloatArrays;
		std::vector<MaterialPropertyData<glm::vec3>> Float3s;
		std::vector<MaterialPropertyData<glm::vec4>> Float4s;
		std::vector<MaterialPropertyData<glm::mat3>> Float3x3s;
		std::vector<MaterialPropertyData<glm::mat4>> Float4x4s;
		std::vector<MaterialPropertyData<std::vector<glm::mat4>>> Float4x4Arrays;

		SERIALIZE_FUNC(
			SERIALIZE(ShaderId),
			SERIALIZE(Textures),
			SERIALIZE(Bools),
			SERIALIZE(Ints),
			SERIALIZE(Floats),
			SERIALIZE(FloatArrays),
			SERIALIZE(Float3s),
			SERIALIZE(Float4s),
			SERIALIZE(Float3x3s),
			SERIALIZE(Float4x4s),
			SERIALIZE(Float4x4Arrays)
		)
	};
}