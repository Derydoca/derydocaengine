#include "EngineComponentsPch.h"
#include "ShaderSubroutineSwitcher.h"

#include <gl\glew.h>
#include <iostream>
#include "Components\MeshRenderer.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	void ShaderSubroutineSwitcher::init()
	{
		setSubroutine(GL_VERTEX_SHADER, m_SubroutineName);
	}

	void ShaderSubroutineSwitcher::deserialize(const YAML::Node& compNode)
	{
		m_SubroutineName = compNode["SubroutineName"].as<std::string>();
	}

	void ShaderSubroutineSwitcher::setSubroutine(unsigned int const& program, std::string const& subroutineName)
	{
		if (subroutineName.empty())
		{
			LOG_ERROR("Unable to set the subroutine for this component because a subroutine name was not supplied.");
			return;
		}

		// Get the shader from the attached mesh renderer
		auto mr = getComponentInChildren<Components::MeshRenderer>();
		if (mr == nullptr)
		{
			LOG_ERROR("No mesh renderer found for ShaderSubroutineSwitcher object.");
			return;
		}
		// Copy the material from the mesh renderer
		auto material = std::make_shared<Rendering::Material>();
		auto mrMat = mr->getMaterial();
		auto mrMaterialId = mr->getMaterialId();
		material->copyFrom(mrMat);
		auto materialResource = std::make_shared<Resources::MaterialResource>();
		materialResource->setData(material);

		// Set the mesh renderer's material to our copy
		mr->setMaterial(materialResource);
		mr->setMaterialId(mrMaterialId);

		// Get the shader from the material
		auto shader = material->getShader();
		if (shader == nullptr)
		{
			LOG_ERROR("Unable to set the subroutine for this component because it does not have a shader assigned to it.");
			return;
		}

		// Ask the shader for the index of the desired subroutine
		GLuint subroutineIndex = shader->getSubroutineIndex(GL_VERTEX_SHADER, subroutineName);

		// Tell the shader to use this subroutine
		material->setSubroutine(program, subroutineIndex);
	}

}
