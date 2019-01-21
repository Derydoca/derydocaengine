#include "EnginePch.h"
#include "TypeNameLookup.h"

namespace DerydocaEngine
{

	TypeNameLookup::TypeNameLookup() :
		m_nameLookup()
	{
	}

	TypeNameLookup::~TypeNameLookup()
	{
	}

	std::string TypeNameLookup::get(std::shared_ptr<Object> object)
	{
		unsigned long typeId = object->getTypeId();
		auto it = m_nameLookup.find(typeId);
		return it == m_nameLookup.end() ? "" : (*it).second;
	}

}
