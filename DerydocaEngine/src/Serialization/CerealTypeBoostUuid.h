#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

namespace cereal
{
    template <class Archive>
    inline std::string save_minimal(Archive const &, boost::uuids::uuid const& uuid)
    {
        std::string val = boost::lexical_cast<std::string>(uuid);
        return val;
    }

    template <class Archive>
    inline void load_minimal(Archive const &, boost::uuids::uuid& uuid, std::string const & val)
    {
        uuid = boost::lexical_cast<boost::uuids::uuid>(val);
    }
}
