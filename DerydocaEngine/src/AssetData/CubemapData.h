#pragma once

namespace DerydocaEngine::AssetData
{
	struct CubemapData
	{
		uuids::uuid texXPos;
		uuids::uuid texXNeg;
		uuids::uuid texYPos;
		uuids::uuid texYNeg;
		uuids::uuid texZPos;
		uuids::uuid texZNeg;

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