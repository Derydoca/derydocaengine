
// Cereal
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#define SERIALIZE(FIELD) CEREAL_NVP(FIELD)
#define SERIALIZE_NAMED(NAME, FIELD) cereal::make_nvp(NAME, FIELD)