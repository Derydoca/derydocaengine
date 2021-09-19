#pragma once
#include <map>
#include <string>
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Rendering {
	class Shader;
}

namespace DerydocaEngine::Rendering
{

	using shaderMap = std::map<uuids::uuid, std::shared_ptr<Shader>>;
	using shaderPathMap = std::map<std::string, uuids::uuid>;

	class ShaderLibrary
	{
	public:
		static ShaderLibrary& getInstance()
		{
			static ShaderLibrary instance;
			return instance;
		}
		void operator=(ShaderLibrary const&) = delete;

		std::shared_ptr<Resources::ShaderResource> findResource(const uuids::uuid& shaderId);
		std::shared_ptr<Shader> find(const uuids::uuid& shaderId);
		std::shared_ptr<Shader> find(const std::string& shaderPath);
		ResourceRef<Resources::ShaderResource> getErrorShader() const { return m_errorShader; }
		void initialize();

		void registerShaderName(std::string const& shaderPath, uuids::uuid const& shaderUuid);
	private:
		ShaderLibrary();
		~ShaderLibrary();
		ShaderLibrary(ShaderLibrary const&) {}

		shaderMap m_shaders;
		shaderPathMap m_shaderPaths;
		ResourceRef<Resources::ShaderResource> m_errorShader;
	};

}