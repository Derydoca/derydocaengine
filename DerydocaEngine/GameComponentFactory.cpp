#include "GameComponentFactory.h"
#include <iostream>
#include "MeshRenderer.h"

GameComponent * GameComponentFactory::CreateGameComponent(string gameComponentType)
{
	// Find a generator function that matches the type passed
	auto it = m_instanceGenerators.find(gameComponentType);

	// If the generator was found
	if (it != m_instanceGenerators.end())
	{
		// Generate it and serve it up
		GameComponent* newGameComponent = it->second();
		return newGameComponent;
	}

	// Otherwise we cannot create a game object and have to return null
	cout << "Unable to create a game object of type '" << gameComponentType << "' because no instance generator exists for it." << endl;
	return nullptr;
}

void GameComponentFactory::registerInstanceGenerator(string gameComponentType, std::function<GameComponent*()> instanceGenerator)
{
	m_instanceGenerators[gameComponentType] = instanceGenerator;
}

GameComponentFactory::GameComponentFactory()
{
	REGINSTANCE(MeshRenderer)
}

GameComponentFactory::~GameComponentFactory()
{
}
