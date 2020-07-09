#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class ShaderSubroutineSwitcher : public Components::GameComponent, Components::SelfRegister<ShaderSubroutineSwitcher>
	{
	public:
		GENINSTANCE(ShaderSubroutineSwitcher);
		SERIALIZE_FUNC_DEFINITIONS;

		ShaderSubroutineSwitcher() {}
		~ShaderSubroutineSwitcher() {}

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);

	private:
		void setSubroutine(unsigned int const& program, std::string const& subroutineName);

		std::string m_SubroutineName;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::ShaderSubroutineSwitcher, 0);
