#include "EnginePch.h"
#include "Helpers/StringUtils.h"
#include <algorithm>

namespace DerydocaEngine
{

	bool endsWith(std::string const& string, std::string const& suffix)
	{
		// If the suffix is longer than the string then there is no way it could end in that string
		if (suffix.length() > string.length())
		{
			return false;
		}

		// Check each character in the suffix to see if it matches the source string's suffix
		size_t stringSuffixIndex = string.length() - suffix.length();
		for (size_t i = 0; i < suffix.length(); i++)
		{
			if (string[stringSuffixIndex + i] != suffix[i])
			{
				return false;
			}
		}

		// If no mismatch was found then the string contains the suffix
		return true;
	}

	std::string getFileExtension(std::string const& path)
	{
		auto p = std::filesystem::path(path);

		auto ext = p.extension();
		if (ext.empty())
		{
			return "";
		}
		else
		{
			auto extStr = ext.string();
			extStr = extStr.substr(1, extStr.length() - 1);
			toLower(extStr);
			return extStr;
		}
	}

	void toLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
	}

}
