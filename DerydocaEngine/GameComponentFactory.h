#pragma once
#include "GameObject.h"
#include <string>
#include <map>

namespace DerydocaEngine::Components
{

	typedef GameComponent* (*gameComponentInstanceGenerator)();

	class GameComponentFactory
	{
	public:
		static GameComponentFactory& getInstance() {
			static GameComponentFactory instance;
			return instance;
		}

		GameComponent * CreateGameComponent(std::string const& gameComponentType);
	private:
		std::map<std::string, gameComponentInstanceGenerator> m_instanceGenerators;
		void registerInstanceGenerator(std::string const& gameComponentType, gameComponentInstanceGenerator const& instanceGenerator);

		GameComponentFactory();
		~GameComponentFactory();
	};

}
