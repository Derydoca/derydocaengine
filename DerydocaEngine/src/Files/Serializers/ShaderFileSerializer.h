#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class ShaderFileSerializer : public FileTypeSerializer
	{
	public:
		ShaderFileSerializer() {};
		~ShaderFileSerializer() {};

		virtual YAML::Node generateResourceNodes(std::string const& filePath);
		virtual Files::FileType getFileType();
		virtual std::shared_ptr<Resources::Resource> loadResourceFromMeta(YAML::Node const& resourceNode);
		virtual void postLoadInitialize(std::shared_ptr<Resources::Resource> resource);
	};

	class GlslShaderFileSerializer : public FileTypeSerializer
	{
	public:
		GlslShaderFileSerializer() {};
		~GlslShaderFileSerializer() {};

		YAML::Node generateResourceNodes(std::string const& filePath);
		Files::FileType getFileType();
		std::shared_ptr<Resources::Resource> loadResourceFromMeta(YAML::Node const& resourceNode);
		virtual void postLoadInitialize(std::shared_ptr<Resources::Resource> resource);
	};

}
