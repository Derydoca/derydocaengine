#include "Timing\Clock.h"

#include <sdl2\SDL.h>

namespace DerydocaEngine::Timing
{

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

	Clock::Clock(unsigned long long int const& startCycle) :
		m_timeCycles(startCycle),
		m_timeScale(1.0f),
		m_paused(false)
	{
		m_startCycle = m_timeCycles;
		m_lastFrameCycle = m_timeCycles;
	}

	void Clock::init()
	{
		s_cyclesPerSecond = (float)SDL_GetPerformanceFrequency();
	}

	float Clock::calcDeltaSeconds(Clock const& other)
	{
		Uint64 dt = m_timeCycles - other.m_timeCycles;
		return cyclesToSeconds(dt);
	}

	void Clock::update()
	{
		if (!m_paused) {
			Uint64 currentFrameCycle = SDL_GetPerformanceCounter();
			float dtRealSeconds = cyclesToSeconds(currentFrameCycle - m_lastFrameCycle);
			Uint64 dtScaledCycles = secondsToCycles(dtRealSeconds * m_timeScale);
			m_deltaTime = dtRealSeconds * m_timeScale;
			m_timeCycles += dtScaledCycles;
			m_lastFrameCycle = currentFrameCycle;
		}
	}

	void Clock::update(float const& dtRealSeconds)
	{
		if (!m_paused) {
			Uint64 dtScaledCycles = secondsToCycles(dtRealSeconds * m_timeScale);
			m_deltaTime = dtRealSeconds * m_timeScale;
			m_timeCycles += dtScaledCycles;
		}
	}

	void Clock::singleStep()
	{
		if (m_paused) {
			// TODO: Make time-step hz a variable
			Uint64 dtScaledCycles = secondsToCycles((1.0f / 30.0f) * m_timeScale);
			m_timeCycles += dtScaledCycles;
		}
	}

	unsigned long long int Clock::getRenderTimeMS()
	{
		unsigned long long int ticks = SDL_GetPerformanceCounter() - m_lastFrameCycle;
		return (int)(cyclesToSeconds(ticks) / 1000);
	}

}