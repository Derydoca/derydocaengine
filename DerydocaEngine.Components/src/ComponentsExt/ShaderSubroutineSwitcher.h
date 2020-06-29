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
		virtual void deserialize(const YAML::Node& compNode);

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_SubroutineName)
		);
	private:
		void setSubroutine(unsigned int const& program, std::string const& subroutineName);

		std::string m_SubroutineName;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::ShaderSubroutineSwitcher);
