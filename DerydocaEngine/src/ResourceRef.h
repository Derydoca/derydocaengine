#pragma once
#include "Resources\Resource.h"
#include "Object.h"
#include "ObjectLibrary.h"
#include <boost\uuid\uuid.hpp>
#include <boost\uuid\uuid_io.hpp>
#include <boost\lexical_cast.hpp>

// A handle holds reference to a resource type and simplifies the serialization of the object
//  to a simple GUID.

namespace DerydocaEngine
{
    template <class ResourceType>
    class ResourceRef
    {
    public:
        ResourceRef() :
            m_Resource()
        {
        }

        ResourceRef(std::shared_ptr<ResourceType> resource) :
            m_Resource(resource)
        {
        }

        template <class Archive>
        std::string save_minimal(Archive const&) const
        {
            if (!m_Resource)
            {
                return "";
            }
            std::string val = boost::lexical_cast<std::string>(m_Resource->getId());
            return val;
        }

        template <class Archive>
        void load_minimal(Archive const&, std::string const& value)
        {
            if (value.length() > 0)
            {
                m_Resource = DerydocaEngine::ObjectLibrary::getInstance().getResource<ResourceType>(value);
            }
        }

        template <class AssetType>
        std::shared_ptr<AssetType> As() const
        {
            return std::static_pointer_cast<AssetType>(m_Resource->getResourceObjectPointer());
        }

        std::shared_ptr<ResourceType> GetSmartPointer() const { return m_Resource; }

        ResourceType* operator->() const {
            return m_Resource.get();
        }

        operator bool() const
        {
            return m_Resource != nullptr;
        }

        void Set(std::shared_ptr<ResourceType> value)
        {
            m_Resource = value;
        }
    private:
        std::shared_ptr<ResourceType> m_Resource;
    };
}
