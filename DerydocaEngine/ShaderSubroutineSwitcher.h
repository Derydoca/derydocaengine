#pragma once
#include <gl\glew.h>
#include "GameComponent.h"
#include "Material.h"

using namespace std;

class ShaderSubroutineSwitcher : public GameComponent
{
public:
	GENINSTANCE(ShaderSubroutineSwitcher);

	ShaderSubroutineSwitcher();
	~ShaderSubroutineSwitcher();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);
private:
	void setSubroutine(GLuint program, string subroutineName);

	string m_subroutineName;
};

