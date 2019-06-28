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
#include "Rectangle.h"
#include "IntRectangle.h"

namespace DerydocaEngine
{

	class YamlTools {
	public:

		static bool getBoolSafe(YAML::Node node, std::string name, bool defaultValue = false) {
			if (node[name])
			{
				return node[name].as<bool>();
			}
			else
			{
				return defaultValue;
			}
		}

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

}

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

	// Add support for GLM ivec4
	template<>
	struct convert<glm::ivec4> {
		static Node encode(const glm::ivec4& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}

		static bool decode(const Node& node, glm::ivec4& vec) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			vec.x = node[0].as<int>();
			vec.y = node[1].as<int>();
			vec.z = node[2].as<int>();
			vec.w = node[3].as<int>();

			return true;
		}
	};

	// Add support for GLM vec3
	template<>
	struct convert<glm::ivec3> {
		static Node encode(const glm::ivec3& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			return node;
		}

		static bool decode(const Node& node, glm::ivec3& vec) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			vec.x = node[0].as<int>();
			vec.y = node[1].as<int>();
			vec.z = node[2].as<int>();

			return true;
		}
	};

	// Add support for GLM vec2
	template<>
	struct convert<glm::ivec2> {
		static Node encode(const glm::ivec2& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node& node, glm::ivec2& vec) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			vec.x = node[0].as<int>();
			vec.y = node[1].as<int>();

			return true;
		}
	};

	// Add support for Colors
	template<>
	struct convert<DerydocaEngine::Color> {
		static Node encode(const DerydocaEngine::Color& color) {
			Node node;
			node.push_back(color.r);
			node.push_back(color.g);
			node.push_back(color.b);
			node.push_back(color.a);
			return node;
		}

		static bool decode(const Node& node, DerydocaEngine::Color& color) {
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
			return true;
		}
	};

	// Add support for Rectangles
	template<>
	struct convert<DerydocaEngine::Rectangle> {
		static Node encode(DerydocaEngine::Rectangle& rect)
		{
			Node node;
			node.push_back(rect.getX());
			node.push_back(rect.getY());
			node.push_back(rect.getWidth());
			node.push_back(rect.getHeight());
			return node;
		}

		static bool decode(const Node& node, DerydocaEngine::Rectangle& rect)
		{
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rect.setX(node[0].as<float>());
			rect.setY(node[1].as<float>());
			rect.setWidth(node[2].as<float>());
			rect.setHeight(node[3].as<float>());

			return true;
		}
	};

	// Add support for IntRectangles
	template<>
	struct convert<DerydocaEngine::IntRectangle> {
		static Node encode(DerydocaEngine::IntRectangle& rect)
		{
			Node node;
			node.push_back(rect.getX());
			node.push_back(rect.getY());
			node.push_back(rect.getWidth());
			node.push_back(rect.getHeight());
			return node;
		}

		static bool decode(const Node& node, DerydocaEngine::IntRectangle& rect)
		{
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rect.setX(node[0].as<int>());
			rect.setY(node[1].as<int>());
			rect.setWidth(node[2].as<int>());
			rect.setHeight(node[3].as<int>());

			return true;
		}
	};
}