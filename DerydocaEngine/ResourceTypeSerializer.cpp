#include "ResourceTypeSerializer.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

boost::uuids::uuid generateUuid()
{
	boost::uuids::random_generator gen;
	boost::uuids::uuid id = gen();
	//delete(&gen);
	return id;
}
