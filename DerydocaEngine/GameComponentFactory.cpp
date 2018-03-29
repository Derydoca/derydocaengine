#include "GameComponentFactory.h"
#include <iostream>
#include "Camera.h"
#include "MeshRenderer.h"
#include "KeyboardMover.h"
#include "MaterialRefresher.h"
#include "Rotator.h"
#include "ScreenshotUtil.h"
#include "Terrain.h"
#include "WasdMover.h"
#include "DebugVisualizer.h"
#include "Light.h"
#include "ShaderSubroutineSwitcher.h"
#include "ImageProjector.h"
#include "EdgeDetectionFilter.h"

// Registers a class to the instance generator map
#define REGINSTANCE(TYPE) \
	registerInstanceGenerator(#TYPE, TYPE::generateInstance);

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
	REGINSTANCE(Camera);
	REGINSTANCE(MeshRenderer);
	REGINSTANCE(KeyboardMover);
	REGINSTANCE(MaterialRefresher);
	REGINSTANCE(Rotator);
	REGINSTANCE(ScreenshotUtil);
	REGINSTANCE(Terrain);
	REGINSTANCE(WasdMover);
	REGINSTANCE(DebugVisualizer);
	REGINSTANCE(Light);
	REGINSTANCE(ShaderSubroutineSwitcher);
	REGINSTANCE(ImageProjector);
	REGINSTANCE(EdgeDetectionFilter);
}

GameComponentFactory::~GameComponentFactory()
{
}
