#include <cereal\archives\json.hpp>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\types\polymorphic.hpp>
#include <cereal\types\base_class.hpp>
#include "Serialization\CerealTypeBoostUuid.h"
#include "Serialization\CerealGlmTypes.h"

#define SERIALIZE_FUNC(...) template<class Archive> \
void serialize(Archive& ar) \
{ \
	ar(##__VA_ARGS__); \
}
#define SERIALIZE_FUNC_BASE(CLASS, ...) template<class Archive> \
void serialize(Archive& ar) \
{ \
	ar(SERIALIZE_BASE(CLASS), ##__VA_ARGS__); \
}
#define SERIALIZE(FIELD) CEREAL_NVP(FIELD)
#define SERIALIZE_NAMED(NAME, FIELD) cereal::make_nvp(NAME, FIELD)
#define SERIALIZE_BASE(CLASS) SERIALIZE_NAMED(std::string("__base_") + #CLASS, cereal::base_class<CLASS>( this ))