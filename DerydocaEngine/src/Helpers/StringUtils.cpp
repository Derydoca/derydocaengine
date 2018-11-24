#include "Helpers\StringUtils.h"
#include <boost/algorithm/string.hpp>

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
		// Find the last instance of a period in the string
		size_t strScanIndex;
		for (strScanIndex = path.length(); strScanIndex > 0; strScanIndex--)
		{
			if (path[strScanIndex] == '.')
			{
				// Move the index forward one so we don't include the period in the returned string
				strScanIndex++;
				break;
			}
		}

		// If a period could not be found, there is no extension
		if (strScanIndex == 0) {
			return "";
		}

		// Otherwise, return the substring of just the extension
		return boost::algorithm::to_lower_copy(path.substr(strScanIndex));
	}

}
