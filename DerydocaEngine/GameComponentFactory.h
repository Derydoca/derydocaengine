#pragma once
#include "GameObject.h"
#include <string>
#include <map>
#include "boost\function.hpp"

using namespace std;

// Registers a class to the instance generator map
#define REGINSTANCE(TYPE) \
	registerInstanceGenerator(#TYPE, TYPE::generateInstance);

class GameComponentFactory
{
public:
	static GameComponentFactory& getInstance() {
		static GameComponentFactory instance;
		return instance;
	}

	GameComponent * CreateGameComponent(string gameComponentType);
private:
	map<string, std::function<GameComponent*()>> m_instanceGenerators;
	void registerInstanceGenerator(string gameComponentType, std::function<GameComponent*()> instanceGenerator);

	GameComponentFactory();
	~GameComponentFactory();
};

