#pragma once
#include "yaml-cpp\yaml.h"
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Color.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

class YamlTools {
public:

	static int getIntSafe(YAML::Node node, std::string name, int defaultValue = 0) {
		if (node[name])
		{
			return node[name].as<std::int32_t>();
		}
		else
		{
			return defaultValue;
		}
	}

	static float getFloatSafe(YAML::Node node, std::string name, float defaultValue = 0) {
		if (node[name])
		{
			return node[name].as<std::float_t>();
		}
		else
		{
			return defaultValue;
		}
	}

};

namespace YAML {

	// Add support for GLM fquat
	template<>
	struct convert<glm::fquat> {
		static Node encode(const glm::fquat& quat) {
			Node node;
			node.push_back(quat.x);
			node.push_back(quat.y);
			node.push_back(quat.z);
			node.push_back(quat.w);
			return node;
		}

		static bool decode(const Node& node, glm::fquat& quat) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			quat.x = node[0].as<float>();
			quat.y = node[1].as<float>();
			quat.z = node[2].as<float>();
			quat.w = node[3].as<float>();

			return true;
		}
	};

	// Add support for GLM vec4
	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& vec) {
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

	// Add support for GLM vec3
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			return node;
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

	// Add support for GLM vec2
	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& vec) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();

			return true;
		}
	};

	// Add support for Colors
	template<>
	struct convert<Color> {
		static Node encode(const Color& color) {
			Node node;
			node.push_back(color.r);
			node.push_back(color.g);
			node.push_back(color.b);
			node.push_back(color.a);
			return node;
		}

		static bool decode(const Node& node, Color& color) {
			if (!node.IsSequence() || node.size() < 3) {
				return false;
			}

			if (node.size() >= 3)
			{
				color.r = node[0].as<float>();
				color.g = node[1].as<float>();
				color.b = node[2].as<float>();
			}
			if (node.size() > 3)
			{
				color.a = node[3].as<float>();
			}

			return true;
		}
	};

	// Add support for Boost UUIDs
	template<>
	struct convert<boost::uuids::uuid> {
		static Node encode(const boost::uuids::uuid& uuid)
		{
			std::string stringUuid = boost::lexical_cast<std::string>(uuid);
			Node node;
			node = stringUuid;
			return node;
		}

		static bool decode(const Node& node, boost::uuids::uuid& uuid)
		{
			std::string stringUuid = node.as<std::string>();
			boost::uuids::string_generator gen;
			uuid = gen(stringUuid);
			//delete(&gen);
			return true;
		}
	};
}