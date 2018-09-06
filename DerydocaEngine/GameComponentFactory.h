#pragma once
#include "GameObject.h"
#include <string>
#include <map>
#include <boost\function.hpp>

using namespace std;

class GameComponentFactory
{
public:
	static GameComponentFactory& getInstance() {
		static GameComponentFactory instance;
		return instance;
	}

	GameComponent * CreateGameComponent(string const& gameComponentType);
private:
	map<string, std::function<GameComponent*()>> m_instanceGenerators;
	void registerInstanceGenerator(string const& gameComponentType, std::function<GameComponent*()> const& instanceGenerator);

	GameComponentFactory();
	~GameComponentFactory();
};

