#include "Clock.h"

float Clock::s_cyclesPerSecond = 1.0f;

Clock::~Clock()
{
}

Clock::Clock(float startTimeSeconds) :
	m_timeCycles(secondsToCycles(startTimeSeconds)),
	m_timeScale(1.0f),
	m_paused(false)
{
}

float Clock::calcDeltaSeconds(const Clock & other)
{
	unsigned long dt = m_timeCycles - other.m_timeCycles;
	return cyclesToSeconds(dt);
}

void Clock::update(float dtRealSeconds)
{
	if (!m_paused) {
		unsigned long dtScaledCycles = secondsToCycles(dtRealSeconds * m_timeScale);
		m_timeCycles += dtScaledCycles;
	}
}

void Clock::singleStep()
{
	if (m_paused) {
		// TODO: Make time-step hz a variable
		unsigned long dtScaledCycles = secondsToCycles((1.0f / 30.0f) * m_timeScale);
		m_timeCycles += dtScaledCycles;
	}
}
