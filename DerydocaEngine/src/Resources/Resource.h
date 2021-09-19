#pragma once
#include <string>
#include "Resources\ResourceType.h"
#include "Object.h"

namespace DerydocaEngine::Resources
{

	struct Resource : public std::enable_shared_from_this<Resource>, public Object
	{
	public:
		Resource();
		Resource(ResourceType type);
		Resource(const uuids::uuid& id, const std::string& sourceFilePath, const std::string& metaFilePath, const ResourceType type);
		virtual ~Resource() {}

		void setData(const std::shared_ptr<void> data) { m_resourceObjectPointer = data; };
		void setFilePaths(const std::string& sourceFilePath, const std::string& metaFilePath);
		void setId(const uuids::uuid& id) { m_ID = id; }
		void setName(const std::string& name) { m_name = name; }
		void setType(const ResourceType type) { m_type = type; }

		void generateAndSetId();
		std::string getMetaFilePath() const { return m_metaFilePath; }
		std::string getName() const { return m_name; }
		std::shared_ptr<void> getResourceObjectPointer();
		std::string getSourceFilePath() const { return m_sourceFilePath; }
		ResourceType getType() const { return m_type; }
		virtual unsigned long getTypeId() const = 0;
		virtual void postLoadInitialize() {}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Object)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Object)
			);
		}

	protected:
		Resource(const std::string& name, const ResourceType type, const std::string& sourceFilePath, const std::string& metaFilePath);
		Resource(const uuids::uuid& id, const std::string& name, const ResourceType type, const std::string& sourceFilePath, const std::string& metaFilePath);

	protected:
		std::string m_name;
		ResourceType m_type;
		std::string m_sourceFilePath;
		std::string m_metaFilePath;
		std::shared_ptr<void> m_resourceObjectPointer;
	};

}
