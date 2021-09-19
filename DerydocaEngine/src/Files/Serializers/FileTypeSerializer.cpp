#include "EnginePch.h"
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	uuids::uuid generateUuid()
	{
		static uuids::uuid_time_generator gen;
		uuids::uuid id = gen();
		return id;
	}

}
