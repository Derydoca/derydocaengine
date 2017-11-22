#include "Clock.h"

float Clock::s_cyclesPerSecond = 1.0f;

Clock::~Clock()
{
}

Clock::Clock() :
	m_timeScale(1.0f),
	m_paused(false)
{
	m_timeCycles = SDL_GetPerformanceCounter();
	m_startCycle = m_timeCycles;
	m_lastFrameCycle = m_timeCycles;
}

Clock::Clock(unsigned long startCycle) :
	m_timeCycles(startCycle),
	m_timeScale(1.0f),
	m_paused(false)
{
	m_startCycle = m_timeCycles;
	m_lastFrameCycle = m_timeCycles;
}

float Clock::calcDeltaSeconds(const Clock & other)
{
	unsigned long dt = m_timeCycles - other.m_timeCycles;
	return cyclesToSeconds(dt);
}

void Clock::update()
{
	if (!m_paused) {
		unsigned long currentFrameCycle = SDL_GetPerformanceCounter();
		float dtRealSeconds = cyclesToSeconds(currentFrameCycle - m_lastFrameCycle);
		unsigned long dtScaledCycles = secondsToCycles(dtRealSeconds * m_timeScale);
		m_deltaTime = dtRealSeconds * m_timeScale;
		m_timeCycles += dtScaledCycles;
		m_lastFrameCycle = currentFrameCycle;
	}
}

void Clock::update(float dtRealSeconds)
{
	if (!m_paused) {
		unsigned long dtScaledCycles = secondsToCycles(dtRealSeconds * m_timeScale);
		m_deltaTime = dtRealSeconds * m_timeScale;
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
