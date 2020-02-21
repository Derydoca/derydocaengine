#pragma once

// Solution from kittikun on Stack Overflow
// https://stackoverflow.com/questions/26989065/how-to-serialize-a-boostuuid-with-cereal

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

namespace cereal
{
    template <class Archive> inline
        void save(Archive& ar, boost::uuids::uuid const& uuid)
    {
        std::string val = boost::lexical_cast<std::string>(uuid);

        ar(val);

        // Other approach, probably better for binary
        //ar(make_size_tag(static_cast<size_type>(uuid.size())));
        //for (auto it = uuid.begin(), end = uuid.end(); it != end; ++it)
        //  ar(*it);
    }

    template <class Archive> inline
        void load(Archive& ar, boost::uuids::uuid& uuid)
    {
        std::string val;

        ar(val);
        uuid = boost::lexical_cast<boost::uuids::uuid>(val);

        // Other approach, probably better for binary
        //size_type size;
        //ar(make_size_tag(size));

        //for (auto it = uuid.begin(), end = uuid.end(); it != end; ++it) {
        //  uint8_t val;
        //  ar(val);
        //  *it = val;
        //}
    }
}
