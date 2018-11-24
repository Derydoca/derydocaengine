#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class ShaderSubroutineSwitcher : public Components::GameComponent, Components::SelfRegister<ShaderSubroutineSwitcher>
	{
	public:
		GENINSTANCE(ShaderSubroutineSwitcher);

		ShaderSubroutineSwitcher() {}
		~ShaderSubroutineSwitcher() {}

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
	private:
		void setSubroutine(unsigned int const& program, std::string const& subroutineName);

		std::string m_subroutineName;
	};

}
