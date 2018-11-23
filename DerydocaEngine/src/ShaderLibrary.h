#pragma once
#include <boost/uuid/uuid.hpp>
#include <map>
#include <string>

namespace DerydocaEngine::Rendering {
	class Shader;
}

namespace DerydocaEngine::Rendering
{

	using shaderMap = std::map<boost::uuids::uuid, Shader*>;
	using shaderPathMap = std::map<std::string, boost::uuids::uuid>;

	class ShaderLibrary
	{
	public:
		static ShaderLibrary& getInstance()
		{
			static ShaderLibrary instance;
			return instance;
		}
		void operator=(ShaderLibrary const&) = delete;

		Shader* find(boost::uuids::uuid const& shaderId);
		Shader* find(std::string const& shaderPath);

		void registerShaderName(std::string const& shaderPath, boost::uuids::uuid const& shaderUuid);
	private:
		ShaderLibrary() {}
		~ShaderLibrary() {}
		ShaderLibrary(ShaderLibrary const&) {}

		shaderMap m_shaders;
		shaderPathMap m_shaderPaths;
	};

}