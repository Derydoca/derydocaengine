#pragma once
#include "GameComponent.h"
#include <string>
#include <map>

namespace DerydocaEngine::Components {

	typedef GameComponent* (*gameComponentInstanceGenerator)();

	class GameComponentFactory
	{
	public:
		GameComponentFactory() = delete;

		static bool registerGenerator(const std::string& name, const gameComponentInstanceGenerator& funcCreate);

		static GameComponent* generate(const std::string& name);

	private:
		static std::map<std::string, gameComponentInstanceGenerator> s_methods;
	};

}
