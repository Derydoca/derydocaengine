#pragma once
#include "GameObject.h"
#include <string>
#include <map>

namespace DerydocaEngine::Components {

	typedef GameComponent* (*gameComponentInstanceGenerator)();

	class GameComponentFactory
	{
	public:
		static GameComponentFactory& getInstance() {
			static GameComponentFactory instance;
			return instance;
		}

		bool registerGenerator(const std::string& name, const gameComponentInstanceGenerator& funcCreate);
		GameComponent* generate(const std::string& name);

	private:
		GameComponentFactory();
		GameComponentFactory(GameComponentFactory const&);
		~GameComponentFactory();

		std::map<std::string, gameComponentInstanceGenerator> s_methods;
	};

}
