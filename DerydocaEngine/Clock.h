#pragma once
#include <sdl2\SDL.h>

class Clock
{
public:
	//Clock(float startTimeSeconds = 0.0f);
	Clock();
	Clock(unsigned long startCycle);
	~Clock();

	inline unsigned long getTimeCycles() const { return m_timeCycles; }
	inline void setPaused(bool wantPaused) { m_paused = wantPaused; }
	inline bool isPaused() const { return m_paused; };
	inline void setTimeScale(float scale) { m_timeScale = scale; }
	inline float getTimeScale() const { return m_timeScale; }
	inline float getDeltaTime() const { return m_deltaTime; }
	inline float getTime() const { return cyclesToSeconds(m_timeCycles); }

	static void init() { s_cyclesPerSecond = (float)SDL_GetPerformanceFrequency(); }
	float calcDeltaSeconds(const Clock& other);
	void update();
	void update(float dtRealSeconds);
	void singleStep();

	static inline unsigned long secondsToCycles(float timeSeconds) {
		return (unsigned long)(timeSeconds * s_cyclesPerSecond);
	}

	static inline float cyclesToSeconds(unsigned long timeCycles) {
		return (float)timeCycles / s_cyclesPerSecond;
	}

private:

	static float s_cyclesPerSecond;

	unsigned long m_timeCycles;
	float m_timeScale;
	bool m_paused;
	float m_deltaTime;
	unsigned long m_startCycle;
	unsigned long m_lastFrameCycle;

};

