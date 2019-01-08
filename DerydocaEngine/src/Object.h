#pragma once
#include <boost\uuid\uuid.hpp>

namespace DerydocaEngine
{

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

	protected:
		boost::uuids::uuid m_id;

	};

}