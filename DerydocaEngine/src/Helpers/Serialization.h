
// Cereal
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

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
#define SERIALIZE_BASE(CLASS) cereal::base_class<CLASS>( this )
#define SERIALIZE(FIELD) CEREAL_NVP(FIELD)
#define SERIALIZE_NAMED(NAME, FIELD) cereal::make_nvp(NAME, FIELD)