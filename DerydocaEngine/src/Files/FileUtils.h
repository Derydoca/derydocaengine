#pragma once
#include <string>
#include <cereal/archives/json.hpp>

namespace DerydocaEngine::Files::Utils
{
	template<class T>
	bool WriteToDisk( T& obj, const std::string& filePath)
	{
		bool result = true;
		try
		{
			std::ofstream fs(filePath);
			cereal::JSONOutputArchive oarchive(fs);
			obj.serialize(oarchive);
		}
		catch (...)
		{
			result = false;
			D_LOG_ERROR("An error occurred while attempting to serialize an object to disk.\nPath: {}", filePath);
		}
		return result;
	}

	template<class T>
	T ReadFromDisk(const std::string& filePath)
	{
		T data;
		try
		{
			std::ifstream fs(filePath);
			cereal::JSONInputArchive iarchive(fs);
			deserializeHelper(data, iarchive);
		}
		catch (...)
		{
			D_LOG_ERROR("An error occurred while attempting to deserialize an object frpm disk.\nPath: {}", filePath);
		}
		return data;
	}

	template<class T>
	T ReadFromDisk2(const std::string& filePath)
	{
		T data;
		try
		{
			std::ifstream fs(filePath);
			cereal::JSONInputArchive iarchive(fs);
			iarchive(data);
		}
		catch (...)
		{
			D_LOG_ERROR("An error occurred while attempting to deserialize an object frpm disk.\nPath: {}", filePath);
		}
		return data;
	}

	template< typename Class, typename Archive,
		typename std::enable_if< cereal::traits::has_member_serialize<Class, Archive>::value>::type * = nullptr>
		inline static void serializeHelper(Class& cl, Archive& ar)
	{
		cl.serialize(ar);
	}

	template< typename Class, typename Archive,
		typename std::enable_if< cereal::traits::has_member_save<Class, Archive>::value>::type * = nullptr>
		inline static void serializeHelper(Class& cl, Archive& ar)
	{
		cl.save(ar);
	}

	// More version could follow for remaining serialization types (external, versioned...)

	template< typename Class, typename Archive,
		typename std::enable_if< cereal::traits::has_member_serialize<Class, Archive>::value>::type * = nullptr>
		inline static void deserializeHelper(Class& cl, Archive& ar)
	{
		cl.serialize(ar);
	}

	template< typename Class, typename Archive,
		typename std::enable_if< cereal::traits::has_member_load<Class, Archive>::value>::type * = nullptr>
		inline static void deserializeHelper(Class& cl, Archive& ar)
	{
		cl.load(ar);
	}

}