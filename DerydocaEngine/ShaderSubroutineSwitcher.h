#pragma once
#include <gl\glew.h>
#include "GameComponent.h"
#include "Material.h"

class ShaderSubroutineSwitcher : public GameComponent
{
public:
	GENINSTANCE(ShaderSubroutineSwitcher);

	ShaderSubroutineSwitcher();
	~ShaderSubroutineSwitcher();

	virtual void init();
	virtual void deserialize(YAML::Node const& compNode);
private:
	void setSubroutine(GLuint const& program, std::string const& subroutineName);

	std::string m_subroutineName;
};

