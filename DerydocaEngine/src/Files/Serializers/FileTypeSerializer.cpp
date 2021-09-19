#include "EnginePch.h"
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	uuids::uuid generateUuid()
	{
		uuids::uuid id = uuids::Generate();
		return id;
	}

}
