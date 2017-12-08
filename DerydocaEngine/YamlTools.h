#pragma once
#include "yaml-cpp\yaml.h"

class YamlTools {
public:

	static int getInt(YAML::Node node, std::string name, int defaultValue = 0) {
		if (node[name])
		{
			return node[name].as<std::int32_t>();
		}
		else
		{
			return defaultValue;
		}
	}

	static int getFloat(YAML::Node node, std::string name, float defaultValue = 0) {
		if (node[name])
		{
			return node[name].as<std::float_t>();
		}
		else
		{
			return defaultValue;
		}
	}

	//static glm::vec3 getVec3(
	//	YAML::Node node,
	//	std::string name,
	//	glm::vec3 defaultValue)
	//{
	//	if (node[name] && node[name].IsSequence() && node[name].size() == 3)
	//	{
	//		YAML::Node vecNode = node[name];
	//		return glm::vec3(
	//			vecNode[0].as<float>(),
	//			vecNode[1].as<float>(),
	//			vecNode[2].as<float>()
	//		);
	//	}
	//	else
	//	{
	//		return defaultValue;
	//	}
	//}

};

namespace YAML {
	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
		}

		static bool Decode(const Node& node, glm::vec4& vec) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
		}

		static bool decode(const Node& node, glm::vec3& vec) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();

			return true;
		}
	};
}