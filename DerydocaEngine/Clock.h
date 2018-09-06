#pragma once
#include <sdl2\SDL.h>
#include "glm\glm.hpp"

class Clock
{
public:
	//Clock(float startTimeSeconds = 0.0f);
	Clock();
	Clock(Uint64 const& startCycle);
	~Clock();

	inline Uint64 getTimeCycles() const { return m_timeCycles; }
	inline void setPaused(bool const& wantPaused) { m_paused = wantPaused; }
	inline bool isPaused() const { return m_paused; };
	inline void setTimeScale(float const& scale) { m_timeScale = scale; }
	inline float getTimeScale() const { return m_timeScale; }
	inline float getDeltaTime() const { if (m_deltaTime < 0) { return 0.01f; } return m_deltaTime; }
	inline float getTime() const { return cyclesToSeconds(m_timeCycles); }

	static void init() { s_cyclesPerSecond = (float)SDL_GetPerformanceFrequency(); }
	float calcDeltaSeconds(Clock const& other);
	void update();
	void update(float const& dtRealSeconds);
	void singleStep();

	static inline Uint64 secondsToCycles(float const& timeSeconds) {
		return (Uint64)(timeSeconds * s_cyclesPerSecond);
	}

	static inline float cyclesToSeconds(Uint64 const& timeCycles) {
		return (float)timeCycles / s_cyclesPerSecond;
	}

	Uint64 getRenderTimeMS() {
		Uint64 ticks = SDL_GetPerformanceCounter() - m_lastFrameCycle;
		return (int)(cyclesToSeconds(ticks) / 1000);
	}

private:

	static float s_cyclesPerSecond;

	Uint64 m_timeCycles;
	float m_timeScale;
	bool m_paused;
	float m_deltaTime;
	Uint64 m_startCycle;
	Uint64 m_lastFrameCycle;

};

