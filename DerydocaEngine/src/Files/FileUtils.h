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
			D_LOG_ERROR("An error occurred while attempting to serialize an object to disk.\Path: {}", filePath);
		}
		return result;
	}
}