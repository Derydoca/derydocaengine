#pragma once

namespace DerydocaEngine::Input
{

	struct Key
	{
	public:
		Key();
		~Key();

		unsigned long getStateChangeTick() const { return m_stateChangeTick; }
		inline bool isDown() const { return m_isDown; }
		void setState(bool const& isDown, unsigned long const& tick);
	private:
		unsigned int m_downTime;
		bool m_isDown;
		bool m_prevDownState;
		unsigned long m_stateChangeTick = 0;
	};

}
