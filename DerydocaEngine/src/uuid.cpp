#include "EnginePch.h"

DerydocaEngine::uuids::uuid::uuid()
{
}

DerydocaEngine::uuids::uuid::uuid(const std::string& stringValue)
{
	load_from_string(stringValue);
}

const std::string DerydocaEngine::uuids::uuid::to_string() const
{
	std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>> sstr;
	sstr << this;
	return sstr.str();
}

const bool DerydocaEngine::uuids::uuid::is_nil() const
{
	for (size_t i = 0; i < 16; i++)
	{
		const uint8_t d = data[i];
		if (d != 0)
		{
			return false;
		}
	}
	return true;
}

const bool DerydocaEngine::uuids::uuid::operator<(const uuid other) const
{
	for (size_t i = 0; i < 16; i++)
	{
		const uint8_t d = data[i];
		const uint8_t o = other.data[i];
		if (d != o)
		{
			return d < o;
		}
	}
	return false;
}

const bool DerydocaEngine::uuids::uuid::operator>(const uuid other) const
{
	for (size_t i = 0; i < 16; i++)
	{
		const uint8_t d = data[i];
		const uint8_t o = other.data[i];
		if (d != o)
		{
			return d > o;
		}
	}
	return false;
}

const bool DerydocaEngine::uuids::uuid::operator==(const uuid& other) const
{
	for (size_t i = 0; i < 16; i++)
	{
		const uint8_t d = data[i];
		const uint8_t o = other.data[i];
		if (d != o)
		{
			return false;
		}
		return true;
	}
}

const bool DerydocaEngine::uuids::uuid::operator!=(const uuid& other) const
{
	return !operator==(other);
}

bool DerydocaEngine::uuids::uuid::operator()(const uuid& left, const uuid& right) const
{
	return left.operator<(right);
}

uint8_t ExtractInt(const std::string& string, int charOffset)
{
	char conv[3] = { '0', '0', 0 };
	conv[0] = string[charOffset];
	conv[1] = string[charOffset + 1];
	uint8_t val = static_cast<uint8_t>(strtoul(&conv[0], NULL, 16));
	return val;
}

void DerydocaEngine::uuids::uuid::load_from_string(const std::string& string)
{
	size_t stringCharOffset = 0;
	size_t uuidIntOffset = 0;

	data[0] = ExtractInt(string, 0);
	data[1] = ExtractInt(string, 2);
	data[2] = ExtractInt(string, 4);
	data[3] = ExtractInt(string, 6);
	data[4] = ExtractInt(string, 9);
	data[5] = ExtractInt(string, 11);
	data[6] = ExtractInt(string, 14);
	data[7] = ExtractInt(string, 16);
	data[8] = ExtractInt(string, 19);
	data[9] = ExtractInt(string, 21);
	data[10] = ExtractInt(string, 24);
	data[11] = ExtractInt(string, 26);
	data[12] = ExtractInt(string, 28);
	data[13] = ExtractInt(string, 30);
	data[14] = ExtractInt(string, 32);
	data[15] = ExtractInt(string, 34);
}

std::ostream& DerydocaEngine::uuids::operator<<(std::ostream& os, const uuid& id)
{
	std::ios_base::fmtflags f(os.flags());

	os << std::hex << std::setfill('0')
		<< std::setw(2) << (int)id.data[0]
		<< std::setw(2) << (int)id.data[1]
		<< std::setw(2) << (int)id.data[2]
		<< std::setw(2) << (int)id.data[3]
		<< '-'
		<< std::setw(2) << (int)id.data[4]
		<< std::setw(2) << (int)id.data[5]
		<< '-'
		<< std::setw(2) << (int)id.data[6]
		<< std::setw(2) << (int)id.data[7]
		<< '-'
		<< std::setw(2) << (int)id.data[8]
		<< std::setw(2) << (int)id.data[9]
		<< '-'
		<< std::setw(2) << (int)id.data[10]
		<< std::setw(2) << (int)id.data[11]
		<< std::setw(2) << (int)id.data[12]
		<< std::setw(2) << (int)id.data[13]
		<< std::setw(2) << (int)id.data[14]
		<< std::setw(2) << (int)id.data[15];

	os.flags(f);

	return os;
}

DerydocaEngine::uuids::uuid DerydocaEngine::uuids::Generate()
{
	uuid id{};

	D_LOG_ERROR("UUID generation needs to be implemented.");

	return id;
}
