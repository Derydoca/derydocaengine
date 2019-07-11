#include "EngineComponentsPch.h"
#include "GameComponent.h"

namespace DerydocaEngine::Components
{

	void GameComponent::destroy(std::shared_ptr<GameObject> objectToDestroy)
	{
		objectToDestroy->destroy();
	}

}