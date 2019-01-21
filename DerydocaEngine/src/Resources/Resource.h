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

		void setId(const boost::uuids::uuid& id) { m_id = id; }
		void setFilePaths(std::string const& sourceFilePath, std::string const& metaFilePath);
		void setType(ResourceType const& type) { m_type = type; }

		ResourceType getType() const { return m_type; }
		std::string getName() const { return m_name; }
		std::string getSourceFilePath() const { return m_sourceFilePath; }
		std::string getMetaFilePath() const { return m_metaFilePath; }
		std::shared_ptr<void> getResourceObjectPointer();
		virtual unsigned long getTypeId() const = 0;
	protected:
		std::string m_name;
		ResourceType m_type;
		std::string m_sourceFilePath;
		std::string m_metaFilePath;
		void* m_resourceObject;
		std::shared_ptr<void> m_resourceObjectPointer;
	};

}
