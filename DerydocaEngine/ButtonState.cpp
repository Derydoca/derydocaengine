#include "ButtonState.h"

ButtonState::ButtonState(unsigned int initialState) :
	m_currentState(initialState),
	m_downStates(0),
	m_upStates(0)
{
}

ButtonState::~ButtonState()
{
}

void ButtonState::Update(unsigned int currentState)
{
	unsigned int changedStates = currentState ^ m_currentState;
	m_downStates = changedStates & currentState;
	m_upStates = changedStates & (~currentState);
}
