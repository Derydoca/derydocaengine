#pragma once
#include <boost\uuid\uuid.hpp>

namespace DerydocaEngine
{

	extern std::atomic_int TypeIdCounter;

	template<typename T>
	int getTypeId() {
		static int id = ++TypeIdCounter;
		return id;
	}

#define REGISTER_TYPE_ID(TYPE) virtual int getTypeId() const { return DerydocaEngine::getTypeId<TYPE>(); }

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

		virtual int getTypeId() const = 0;

	protected:
		boost::uuids::uuid m_id;

	};

}