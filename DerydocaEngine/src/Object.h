#pragma once
#include "DerydocaEngineCommonPch.h"
#include <boost\uuid\uuid.hpp>

namespace DerydocaEngine
{

	template<typename T>
	unsigned long getTypeId() {
		static unsigned long id = static_cast<unsigned long>(typeid(T).hash_code());
		return id;
	}

#define REGISTER_TYPE_ID(TYPE) virtual unsigned long getTypeId() const { return DerydocaEngine::getTypeId<TYPE>(); }

	class Object
	{
	public:
		Object() :
			m_id()
		{
		}

		Object(boost::uuids::uuid id) :
			m_id(id)
		{
		}

		boost::uuids::uuid getId() const { return m_id; }

		virtual unsigned long getTypeId() const = 0;

		SERIALIZE_FUNC(
			SERIALIZE(m_id)
		);

	protected:
		boost::uuids::uuid m_id;

	};

}