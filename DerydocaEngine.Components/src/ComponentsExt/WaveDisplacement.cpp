#include "EngineComponentsPch.h"
#include "WaveDisplacement.h"

#include "Rendering\Material.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	WaveDisplacement::WaveDisplacement() :
		m_k(1.0f),
		m_velocity(1.0f),
		m_amplitude(0.5f),
		m_time(0.0f),
		m_material()
	{
	}

	WaveDisplacement::~WaveDisplacement()
	{
	}

	void WaveDisplacement::init()
	{
		// Get reference to the material on this object
		auto mr = getComponentInChildren<Components::MeshRenderer>();
		assert(mr);
		if (mr)
		{
			m_material = mr->getMaterial();
		}
		assert(m_material);
	}

	void WaveDisplacement::update(const float deltaTime)
	{
		m_time += deltaTime;

		// Set the wave variables
		m_material->setFloat("K", m_k);
		m_material->setFloat("Velocity", m_velocity);
		m_material->setFloat("Amp", m_amplitude);
	}

	void WaveDisplacement::preRender()
	{
		// Update the shader's time variable
		m_material->setFloat("Time", m_time);
	}

	void WaveDisplacement::deserialize(const YAML::Node& compNode)
	{
		YAML::Node kNode = compNode["k"];
		if (kNode)
		{
			m_k = kNode.as<float>();
		}

		YAML::Node velocityNode = compNode["velocity"];
		if (velocityNode)
		{
			m_velocity = velocityNode.as<float>();
		}

		YAML::Node amplitudeNode = compNode["amplitude"];
		if (amplitudeNode)
		{
			m_amplitude = amplitudeNode.as<float>();
		}
	}

}
