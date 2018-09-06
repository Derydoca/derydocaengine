#include "ShaderSubroutineSwitcher.h"
#include <iostream>
#include "MeshRenderer.h"
#include "Shader.h"

ShaderSubroutineSwitcher::ShaderSubroutineSwitcher()
{
}

ShaderSubroutineSwitcher::~ShaderSubroutineSwitcher()
{
}

void ShaderSubroutineSwitcher::init()
{
	setSubroutine(GL_VERTEX_SHADER, m_subroutineName);
}

void ShaderSubroutineSwitcher::deserialize(YAML::Node const& compNode)
{
	m_subroutineName = compNode["SubroutineName"].as<string>();
}

void ShaderSubroutineSwitcher::setSubroutine(GLuint const& program, string const& subroutineName)
{
	if (subroutineName.empty())
	{
		cout << "Unable to set the subroutine for this component because a subroutine name was not supplied." << endl;
		return;
	}

	// Get the shader from the attached mesh renderer
	MeshRenderer* mr = getComponent<MeshRenderer>();
	if (mr == nullptr)
	{
		cout << "No mesh renderer found for ShaderSubroutineSwitcher object.";
		return;
	}
	// Get the shader attached to the mesh renderer
	Material* material = mr->getMaterial();
	Shader* shader = material->getShader();
	if (shader == nullptr)
	{
		cout << "Unable to set the subroutine for this component because it does not have a shader assigned to it." << endl;
		return;
	}

	// Ask the shader for the index of the desired subroutine
	GLuint subroutineIndex = shader->getSubroutineIndex(GL_VERTEX_SHADER, subroutineName);

	// Tell the shader to use this subroutine
	material->setSubroutine(program, subroutineIndex);
}
