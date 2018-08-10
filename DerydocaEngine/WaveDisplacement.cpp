#include "WaveDisplacement.h"
#include "MeshRenderer.h"

WaveDisplacement::WaveDisplacement()
{
}

WaveDisplacement::~WaveDisplacement()
{
}

void WaveDisplacement::init()
{
	// Get reference to the material on this object
	MeshRenderer* mr = getComponent<MeshRenderer>();
	assert(mr);
	if (mr)
	{
		m_material = mr->getMaterial();
	}
	assert(m_material);

	// Set the wave variables
	m_material->setFloat("K", m_k);
	m_material->setFloat("Velocity", m_velocity);
	m_material->setFloat("Amp", m_amplitude);
}

void WaveDisplacement::update(float deltaTime)
{
	m_time += deltaTime;
}

void WaveDisplacement::preRender()
{
	// Update the shader's time variable
	m_material->setFloat("Time", m_time);
}

void WaveDisplacement::deserialize(YAML::Node compNode)
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