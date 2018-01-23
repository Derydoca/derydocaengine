#pragma once
#include <string>

bool endsWith(const std::string& string, const std::string& suffix)
{
	// If the suffix is longer than the string then there is no way it could end in that string
	if (suffix.length() > string.length())
	{
		return false;
	}

	// Check each character in the suffix to see if it matches the source string's suffix
	int stringSuffixIndex = string.length() - suffix.length();
	for (int i = 0; i < suffix.length(); i++)
	{
		if (string[stringSuffixIndex + i] != suffix[i])
		{
			return false;
		}
	}

	// If no mismatch was found then the string contains the suffix
	return true;
}