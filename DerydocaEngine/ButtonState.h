#pragma once
class ButtonState
{
public:
	ButtonState(unsigned int initialState);
	~ButtonState();

	void update(unsigned int currentState);
	inline bool isKeyDown(unsigned int mask) { return mask & m_currentState != 0; }
	inline bool isKeyUp(unsigned int mask) { return mask & m_currentState == 0; }
	inline bool isKeyDownFrame(unsigned int mask) { return mask & m_downStates != 0; }
	inline bool isKeyUpFrame(unsigned int mask) { return mask & m_upStates != 0; }
private:
	unsigned int m_currentState;
	unsigned int m_downStates;
	unsigned int m_upStates;
};

