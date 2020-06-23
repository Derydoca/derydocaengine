#pragma once
#include "Resources\Resource.h"
#include <string>

namespace DerydocaEngine::Resources
{

	struct ShaderResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(ShaderResource);

		ShaderResource() :
			Resource(ResourceType::ShaderResourceType),
			m_rawShaderName(),
			m_fragmentShaderSource(),
			m_geometryShaderSource()
		{}
		~ShaderResource() {}

		void setRawShaderName(std::string const& rawShaderName) { m_rawShaderName = rawShaderName; }
		void setFragmentShaderSource(std::string const& fragmentShaderSource) { m_fragmentShaderSource = fragmentShaderSource; }
		void setGeometryShaderSource(std::string const& geometryShaderSource) { m_geometryShaderSource = geometryShaderSource; }

		std::string getRawShaderName() const { return m_rawShaderName; }
		std::string getVertexShaderLocation() const { return m_sourceFilePath; }
		std::string getFragmentShaderSource() const { return m_fragmentShaderSource; }
		std::string getGeometryShaderSource() const { return m_geometryShaderSource; }

		virtual void postLoadInitialize();

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource)
			);
		}


	private:
		std::string m_rawShaderName;
		std::string m_fragmentShaderSource;
		std::string m_geometryShaderSource;
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::ShaderResource);
