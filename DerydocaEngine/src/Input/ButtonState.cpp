#include "EnginePch.h"
#include "Input\ButtonState.h"

namespace DerydocaEngine::Input
{

	ButtonState::ButtonState(unsigned int const& initialState) :
		m_currentState(initialState),
		m_downStates(0),
		m_upStates(0)
	{
	}

	ButtonState::~ButtonState()
	{
	}

	void ButtonState::update(unsigned int const& currentState)
	{
		unsigned int changedStates = currentState ^ m_currentState;
		m_downStates = changedStates & currentState;
		m_upStates = changedStates & (~currentState);
	}

}
