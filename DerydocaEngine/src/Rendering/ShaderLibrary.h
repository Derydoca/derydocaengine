#pragma once
#include <boost/uuid/uuid.hpp>
#include <map>
#include <string>

namespace DerydocaEngine::Rendering {
	class Shader;
}

namespace DerydocaEngine::Rendering
{

	using shaderMap = std::map<boost::uuids::uuid, std::shared_ptr<Shader>>;
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

		std::shared_ptr<Shader> find(const boost::uuids::uuid& shaderId);
		std::shared_ptr<Shader> find(const std::string& shaderPath);
		std::shared_ptr<Shader> getErrorShader() const { return m_errorShader; }
		void initialize();

		void registerShaderName(std::string const& shaderPath, boost::uuids::uuid const& shaderUuid);
	private:
		ShaderLibrary();
		~ShaderLibrary();
		ShaderLibrary(ShaderLibrary const&) {}

		shaderMap m_shaders;
		shaderPathMap m_shaderPaths;
		std::shared_ptr<Shader> m_errorShader;
	};

}