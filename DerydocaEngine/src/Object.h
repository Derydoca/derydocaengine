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
			m_ID()
		{
		}

		Object(boost::uuids::uuid id) :
			m_ID(id)
		{
		}

		virtual ~Object() {};

		boost::uuids::uuid getId() const { return m_ID; }

		virtual unsigned long getTypeId() const = 0;

		SERIALIZE_FUNC(SERIALIZE(m_ID));

	protected:
		boost::uuids::uuid m_ID;
	};

}