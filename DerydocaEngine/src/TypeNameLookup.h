#pragma once
#include "Object.h"

namespace DerydocaEngine
{

	class TypeNameLookup
	{
	public:
		static TypeNameLookup& getInstace() {
			static TypeNameLookup m_instance;
			return m_instance;
		}

		template<typename T>
		bool registerType(std::string typeName)
		{
			unsigned int typeId = getTypeId<T>();
			m_nameLookup[typeId] = typeName;
			return true;
		}

		std::string get(std::shared_ptr<Object> object);

	private:
		TypeNameLookup();
		~TypeNameLookup();
		TypeNameLookup(const TypeNameLookup&);

		std::unordered_map<unsigned int, std::string> m_nameLookup;
	};

}
