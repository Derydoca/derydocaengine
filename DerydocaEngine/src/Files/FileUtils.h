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
			cereal::JSONOutputArchive iarchive(fs);
			data.load(iarchive);
		}
		catch (...)
		{
			D_LOG_ERROR("An error occurred while attempting to deserialize an object frpm disk.\nPath: {}", filePath);
		}
		return result;
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
}