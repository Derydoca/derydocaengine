#include <cereal\archives\json.hpp>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\types\polymorphic.hpp>
#include <cereal\types\base_class.hpp>
#include "Serialization\CerealTypeBoostUuid.h"
#include "Serialization\CerealGlmTypes.h"

#define SERIALIZE_FUNC_DEFINITIONS template<class Archive>\
void load(Archive& archive);\
template<class Archive>\
void save(Archive& archive) const;

#define SERIALIZE_FUNC_LOAD(ARCHIVE, CLASSNAME) template<class Archive>\
void CLASSNAME::load(Archive& ARCHIVE)

#define SERIALIZE_FUNC_SAVE(ARCHIVE, CLASSNAME) template<class Archive>\
void CLASSNAME::save(Archive& ARCHIVE) const

#define SERIALIZE_FUNC(...) template<class Archive>\
void save(Archive& archive) const\
{\
	archive(##__VA_ARGS__);\
}\
template<class Archive>\
void load(Archive& archive)\
{\
	archive(##__VA_ARGS__);\
}

#define SERIALIZE_FUNC_BASE(CLASS, ...) template<class Archive>\
void save(Archive& archive) const\
{\
	archive(SERIALIZE_BASE(CLASS), ##__VA_ARGS__);\
}\
template<class Archive>\
void load(Archive& archive)\
{\
	archive(SERIALIZE_BASE(CLASS), ##__VA_ARGS__);\
}

#define SERIALIZE(FIELD) CEREAL_NVP(FIELD)
#define SERIALIZE_NAMED(NAME, FIELD) cereal::make_nvp(NAME, FIELD)
#define SERIALIZE_BASE(CLASS) SERIALIZE_NAMED(std::string("__base_") + #CLASS, cereal::base_class<CLASS>( this ))

#define REGISTER_SERIALIZED_TYPE(CLASS) CEREAL_REGISTER_TYPE(CLASS)
