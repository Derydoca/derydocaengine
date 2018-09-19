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
#include "GaussianBlurFilter.h"
#include "BloomFilter.h"
#include "GammaCorrectionFilter.h"
#include "MultisampleSwitch.h"
#include "ParticleSystem.h"
#include "BezierCurveRenderer.h"
#include "TessellatingQuad.h"
#include "TessellatedMeshRenderer.h"
#include "NoiseTexture.h"
#include "WoodSliceTexture.h"
#include "NightVisionFilter.h"
#include "WaveDisplacement.h"
#include "ParticleFountain.h"
#include "ParticleContinuousFountain.h"
#include "ParticleInstanced.h"
#include "TextRenderer.h"
#include "FrameStats.h"
#include "SpriteRenderer.h"

// Registers a class to the instance generator map
#define REGINSTANCE(TYPE) \
	registerInstanceGenerator(#TYPE, TYPE::generateInstance);

DerydocaEngine::Components::GameComponent * GameComponentFactory::CreateGameComponent(std::string const& gameComponentType)
{
	// Find a generator function that matches the type passed
	auto it = m_instanceGenerators.find(gameComponentType);

	// If the generator was found
	if (it != m_instanceGenerators.end())
	{
		// Generate it and serve it up
		DerydocaEngine::Components::GameComponent* newGameComponent = it->second();
		return newGameComponent;
	}

	// Otherwise we cannot create a game object and have to return null
	std::cout << "Unable to create a game object of type '" << gameComponentType << "' because no instance generator exists for it.\n";
	return nullptr;
}

void GameComponentFactory::registerInstanceGenerator(std::string const& gameComponentType, std::function<DerydocaEngine::Components::GameComponent*()> const& instanceGenerator)
{
	size_t scopeOperatorLoc = gameComponentType.find_last_of(':');
	if (scopeOperatorLoc > 0)
	{
		std::string typeWithoutNamespace = gameComponentType.substr(scopeOperatorLoc + 1);
		m_instanceGenerators[typeWithoutNamespace] = instanceGenerator;
	}
	else
	{
		m_instanceGenerators[gameComponentType] = instanceGenerator;
	}
}

GameComponentFactory::GameComponentFactory()
{
	REGINSTANCE(DerydocaEngine::Components::Camera);
	REGINSTANCE(DerydocaEngine::Components::MeshRenderer);
	REGINSTANCE(DerydocaEngine::Components::KeyboardMover);
	REGINSTANCE(DerydocaEngine::Components::MaterialRefresher);
	REGINSTANCE(DerydocaEngine::Components::Rotator);
	REGINSTANCE(DerydocaEngine::Components::ScreenshotUtil);
	REGINSTANCE(DerydocaEngine::Components::Terrain);
	REGINSTANCE(DerydocaEngine::Components::WasdMover);
	REGINSTANCE(DerydocaEngine::Debug::DebugVisualizer);
	REGINSTANCE(DerydocaEngine::Components::Light);
	REGINSTANCE(DerydocaEngine::Ext::ShaderSubroutineSwitcher);
	REGINSTANCE(DerydocaEngine::Ext::ImageProjector);
	REGINSTANCE(DerydocaEngine::Ext::EdgeDetectionFilter);
	REGINSTANCE(DerydocaEngine::Ext::GaussianBlurFilter);
	REGINSTANCE(DerydocaEngine::Ext::BloomFilter);
	REGINSTANCE(DerydocaEngine::Ext::GammaCorrectionFilter);
	REGINSTANCE(DerydocaEngine::Ext::MultisampleSwitch);
	REGINSTANCE(DerydocaEngine::Ext::ParticleSystem);
	REGINSTANCE(DerydocaEngine::Ext::BezierCurveRenderer);
	REGINSTANCE(DerydocaEngine::Ext::TessellatingQuad);
	REGINSTANCE(DerydocaEngine::Ext::TessellatedMeshRenderer);
	REGINSTANCE(DerydocaEngine::Ext::NoiseTexture);
	REGINSTANCE(DerydocaEngine::Ext::WoodSliceTexture);
	REGINSTANCE(DerydocaEngine::Ext::NightVisionFilter);
	REGINSTANCE(DerydocaEngine::Ext::WaveDisplacement);
	REGINSTANCE(DerydocaEngine::Ext::ParticleFountain);
	REGINSTANCE(DerydocaEngine::Ext::ParticleContinuousFountain);
	REGINSTANCE(DerydocaEngine::Ext::ParticleInstanced);
	REGINSTANCE(DerydocaEngine::Components::TextRenderer);
	REGINSTANCE(DerydocaEngine::Components::FrameStats);
	REGINSTANCE(DerydocaEngine::Components::SpriteRenderer);
}

GameComponentFactory::~GameComponentFactory()
{
}
