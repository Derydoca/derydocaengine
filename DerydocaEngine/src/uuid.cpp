#include "EnginePch.h"
#include <chrono>
#include <random>
#include <array>

#ifdef _WIN32
#include <Windows.h>
#include <Iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#endif

namespace DerydocaEngine::uuids
{
	uuid::uuid()
		: data({})
	{
	}

	uuid::uuid(const std::array<uint8_t, 16>& rawID)
	{
		memcpy(&data, &rawID[0], 16);
	}

	uuid::uuid(const std::string& stringValue)
	{
		load_from_string(stringValue);
	}

	const std::string uuid::to_string() const
	{
		std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>> sstr;
		sstr << *this;
		return sstr.str();
	}

	const bool uuid::is_nil() const
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

	const bool uuid::operator<(const uuid other) const
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

	const bool uuid::operator>(const uuid other) const
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

	const bool uuid::operator==(const uuid& other) const
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

	const bool uuid::operator!=(const uuid& other) const
	{
		return !operator==(other);
	}

	bool uuid::operator()(const uuid& left, const uuid& right) const
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

	void uuid::load_from_string(const std::string& string)
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

	uuid uuid_time_generator::operator()()
	{
		if (get_mac_address())
		{
			auto start = std::chrono::system_clock::from_time_t(time_t(-12219292800));
			auto diff = std::chrono::system_clock::now() - start;
			auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
			auto tm = ns / 100;
			auto ptm = reinterpret_cast<uint8_t*>(&tm);

			static std::mt19937 clock_gen(std::random_device{}());
			static std::uniform_int_distribution<unsigned short> clock_dis;
			static std::atomic_ushort clock_sequence = clock_dis(clock_gen);

			auto clock_seq = clock_sequence++;
			std::array<uint8_t, 16> data{};

			memcpy(&data[0], ptm + 4, 4);
			memcpy(&data[4], ptm + 2, 2);
			memcpy(&data[6], ptm, 2);
			memcpy(&data[8], &clock_seq, 2);

			data[8] &= 0xBF;
			data[8] |= 0x80;
			data[6] &= 0x1F;
			data[6] |= 0x10;

			memcpy(&data[10], &device_address.value()[0], 6);

			return uuid(data);
		}

		return {};
	}

	bool uuid_time_generator::get_mac_address()
	{
		if (device_address.has_value())
		{
			return true;
		}

#ifdef _WIN32
		DWORD len = 0;
		auto ret = GetAdaptersInfo(nullptr, &len);
		if (ret != ERROR_BUFFER_OVERFLOW) return false;
		std::vector<unsigned char> buf(len);
		auto pips = reinterpret_cast<PIP_ADAPTER_INFO>(&buf.front());
		ret = GetAdaptersInfo(pips, &len);
		if (ret != ERROR_SUCCESS) return false;
		mac_address addr;
		std::copy(pips->Address, pips->Address + 6, std::begin(addr));
		device_address = addr;
#elif
		D_LOG_CRITICAL("Unable to find the MAC address for this device!");
		return false;
#endif

		return device_address.has_value();
	}

}
