#include "Components\GameComponentFactory.h"

namespace DerydocaEngine::Components
{

	bool GameComponentFactory::registerGenerator(const std::string& name, const gameComponentInstanceGenerator& funcCreate)
	{
		auto it = s_methods.find(name);
		if (it == s_methods.end()) {
			s_methods[name] = funcCreate;
			return true;
		}

		return false;
	}

	std::shared_ptr<GameComponent> GameComponentFactory::generate(const std::string & name)
	{
		auto it = s_methods.find(name);
		if (it != s_methods.end()) {
			return it->second();
		}

		return nullptr;
	}

	GameComponentFactory::GameComponentFactory()
	{
	}

	GameComponentFactory::~GameComponentFactory()
	{
	}

}
