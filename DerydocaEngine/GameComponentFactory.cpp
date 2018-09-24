#include "GameComponentFactory.h"

namespace DerydocaEngine::Components
{

	std::map<std::string, gameComponentInstanceGenerator> GameComponentFactory::s_methods;

	bool GameComponentFactory::registerGenerator(const std::string& name, const gameComponentInstanceGenerator& funcCreate)
	{
		auto it = s_methods.find(name);
		if (it == s_methods.end()) {
			s_methods[name] = funcCreate;
			return true;
		}

		return false;
	}

	GameComponent * GameComponentFactory::generate(const std::string & name)
	{
		auto it = s_methods.find(name);
		if (it != s_methods.end()) {
			return it->second();
		}

		return nullptr;
	}

}
