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

	GameComponent * CreateGameComponent(std::string const& gameComponentType);
private:
	std::map<std::string, std::function<GameComponent*()>> m_instanceGenerators;
	void registerInstanceGenerator(std::string const& gameComponentType, std::function<GameComponent*()> const& instanceGenerator);

	GameComponentFactory();
	~GameComponentFactory();
};

