#include "EngineComponentsPch.h"
#include "WaveDisplacement.h"

#include "Rendering\Material.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	WaveDisplacement::WaveDisplacement() :
		m_K(1.0f),
		m_Velocity(1.0f),
		m_Amplitude(0.5f),
		m_Time(0.0f),
		m_Material()
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
			m_Material = mr->getMaterial();
		}
		assert(m_Material);
	}

	void WaveDisplacement::update(const float deltaTime)
	{
		m_Time += deltaTime;

		// Set the wave variables
		m_Material->setFloat("K", m_K);
		m_Material->setFloat("Velocity", m_Velocity);
		m_Material->setFloat("Amp", m_Amplitude);
	}

	void WaveDisplacement::preRender()
	{
		// Update the shader's time variable
		m_Material->setFloat("Time", m_Time);
	}

	void WaveDisplacement::deserialize(const YAML::Node& compNode)
	{
		YAML::Node kNode = compNode["k"];
		if (kNode)
		{
			m_K = kNode.as<float>();
		}

		YAML::Node velocityNode = compNode["velocity"];
		if (velocityNode)
		{
			m_Velocity = velocityNode.as<float>();
		}

		YAML::Node amplitudeNode = compNode["amplitude"];
		if (amplitudeNode)
		{
			m_Amplitude = amplitudeNode.as<float>();
		}
	}

}
