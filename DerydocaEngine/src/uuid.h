#pragma once
#include "DerydocaEngineCommonPch.h"
#include <cstdint>
#include <iostream>
#include <optional>

using mac_address = std::array<unsigned char, 6>;

namespace DerydocaEngine::uuids
{
	struct uuid
	{
	public:
		uuid();
		uuid(const std::array<uint8_t, 16>& rawID);
		uuid(const std::string& stringValue);

		const std::string to_string() const;
		const bool is_nil() const;

		const bool operator<(const uuid other) const;
		const bool operator>(const uuid other) const;
		const bool operator==(const uuid& other) const;
		const bool operator!=(const uuid& other) const;
		bool operator() (const uuid& left, const uuid& right) const;

		friend std::ostream& operator<<(std::ostream& os, const uuid& id);

		template<class CharT = char,
			class Traits = std::char_traits<CharT>,
			class Allocator = std::allocator<CharT>>
			inline std::basic_string<CharT, Traits, Allocator> to_string(uuid const& id)
		{
			std::basic_stringstream<CharT, Traits, Allocator> sstr;
			sstr << id;
			return sstr.str();
		}

		template<class Archive>
		std::string save_minimal(Archive const&) const
		{
			return to_string();
		}

		template<class Archive>
		void load_minimal(Archive const&, std::string const& val)
		{
			load_from_string(val);
		}

	private:
		void load_from_string(const std::string& string);

	private:
		std::array<uint8_t, 16> data;
	};

	class uuid_time_generator
	{
	public:
		uuid operator()();

	private:
		bool get_mac_address();

	private:
		std::optional<mac_address> device_address;

	};
};