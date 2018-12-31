#pragma once

namespace DerydocaEngine::Timing
{

	class Clock
	{
	public:
		Clock();
		Clock(unsigned long long int const& startCycle);
		~Clock();

		inline unsigned long long int getTimeCycles() const { return m_timeCycles; }
		inline void setPaused(bool const& wantPaused) { m_paused = wantPaused; }
		inline bool isPaused() const { return m_paused; };
		inline void setTimeScale(float const& scale) { m_timeScale = scale; }
		inline float getTimeScale() const { return m_timeScale; }
		inline float getDeltaTime() const { if (m_deltaTime < 0) { return 0.01f; } return m_deltaTime; }
		inline float getTime() const { return cyclesToSeconds(m_timeCycles); }

		static void init();
		float calcDeltaSeconds(Clock const& other);
		void update();
		void update(const float dtRealSeconds);
		void singleStep();

		static inline unsigned long long int secondsToCycles(float const& timeSeconds) {
			return (unsigned long long int)(timeSeconds * s_cyclesPerSecond);
		}

		static inline float cyclesToSeconds(unsigned long long int const& timeCycles) {
			return (float)timeCycles / s_cyclesPerSecond;
		}

		unsigned long long int getRenderTimeMS();

	private:

		static float s_cyclesPerSecond;

		unsigned long long int m_timeCycles;
		float m_timeScale;
		bool m_paused;
		float m_deltaTime;
		unsigned long long int m_startCycle;
		unsigned long long int m_lastFrameCycle;

	};

}
