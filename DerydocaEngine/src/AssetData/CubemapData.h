#pragma once
#include "boost/uuid/uuid.hpp"

namespace DerydocaEngine::AssetData
{
	struct CubemapData
	{
		boost::uuids::uuid texXPos;
		boost::uuids::uuid texXNeg;
		boost::uuids::uuid texYPos;
		boost::uuids::uuid texYNeg;
		boost::uuids::uuid texZPos;
		boost::uuids::uuid texZNeg;

		SERIALIZE_FUNC(
			SERIALIZE(texXPos),
			SERIALIZE(texXNeg),
			SERIALIZE(texYPos),
			SERIALIZE(texYNeg),
			SERIALIZE(texZPos),
			SERIALIZE(texZNeg)
		)
	};
}