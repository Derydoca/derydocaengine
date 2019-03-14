#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include "Resources\ResourceType.h"
#include "Object.h"

namespace DerydocaEngine::Resources
{

	struct Resource : public std::enable_shared_from_this<Resource>, public Object
	{
	public:
		Resource();
		Resource(boost::uuids::uuid const& id, std::string const& sourceFilePath, std::string const& metaFilePath, ResourceType const& type);

		void setData(std::shared_ptr<void> data) { m_resourceObjectPointer = data; };
		void setFilePaths(std::string const& sourceFilePath, std::string const& metaFilePath);
		void setId(const boost::uuids::uuid& id) { m_id = id; }
		void setName(const std::string& name) { m_name = name; }
		void setType(ResourceType const& type) { m_type = type; }

		std::string getMetaFilePath() const { return m_metaFilePath; }
		std::string getName() const { return m_name; }
		std::shared_ptr<void> getResourceObjectPointer();
		std::string getSourceFilePath() const { return m_sourceFilePath; }
		ResourceType getType() const { return m_type; }
		virtual unsigned long getTypeId() const = 0;

	protected:
		Resource(const std::string& name, ResourceType type, const std::string& sourceFilePath, const std::string& metaFilePath);
		Resource(const boost::uuids::uuid& id, const std::string& name, ResourceType type, const std::string& sourceFilePath, const std::string& metaFilePath);

	protected:
		std::string m_name;
		ResourceType m_type;
		std::string m_sourceFilePath;
		std::string m_metaFilePath;
		std::shared_ptr<void> m_resourceObjectPointer;
	};

}
