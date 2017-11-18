#pragma once
class ButtonState
{
public:
	ButtonState(unsigned int initialState);
	~ButtonState();

	void Update(unsigned int currentState);
	inline bool IsKeyDown(unsigned int mask) { return mask & m_currentState != 0; }
	inline bool IsKeyUp(unsigned int mask) { return mask & m_currentState == 0; }
	inline bool IsKeyDownFrame(unsigned int mask) { return mask & m_downStates != 0; }
	inline bool IsKeyUpFrame(unsigned int mask) { return mask & m_upStates != 0; }
private:
	unsigned int m_currentState;
	unsigned int m_downStates;
	unsigned int m_upStates;
};

