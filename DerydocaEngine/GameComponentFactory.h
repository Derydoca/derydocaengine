#pragma once
#include "GameObject.h"
#include <string>
#include <map>
#include <boost\function.hpp>

class GameComponentFactory
{
public:
	static GameComponentFactory& getInstance() {
		static GameComponentFactory instance;
		return instance;
	}

	DerydocaEngine::Components::GameComponent * CreateGameComponent(std::string const& gameComponentType);
private:
	std::map<std::string, std::function<DerydocaEngine::Components::GameComponent*()>> m_instanceGenerators;
	void registerInstanceGenerator(std::string const& gameComponentType, std::function<DerydocaEngine::Components::GameComponent*()> const& instanceGenerator);

	GameComponentFactory();
	~GameComponentFactory();
};

