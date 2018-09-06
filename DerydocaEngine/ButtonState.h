#pragma once
class ButtonState
{
public:
	ButtonState(unsigned int const& initialState);
	~ButtonState();

	/*
	Updates the state of this button. This should be called once per frame.

	@param currentState Packed list of booleans in an unsigned int that represents the current state of the device.
	*/
	void update(unsigned int const& currentState);

	/*
	Returns true when the button is down.

	@mask Bitmask of the button to query.
	@return Returns true when button is pressed.
	*/
	inline bool isKeyDown(unsigned int const& mask) const { return (mask & m_currentState) != 0; }

	/*
	Returns true when the button is up.

	@mask Bitmask of the button to query.
	@return Returns true when button is not pressed.
	*/
	inline bool isKeyUp(unsigned int const& mask) const { return (mask & m_currentState) == 0; }

	/*
	Returns true on the frame that the button switches to a down position.

	@mask Bitmask of the button to query.
	@return Returns true on the frame the state transitions to down.
	*/
	inline bool isKeyDownFrame(unsigned int const& mask) const { return (mask & m_downStates) != 0; }

	/*
	Returns true on the frame that the button switches to an up position.

	@mask Bitmask of the button to query.
	@return Returns true on the frame the state transitions to up.
	*/
	inline bool isKeyUpFrame(unsigned int const& mask) const { return (mask & m_upStates) != 0; }
private:
	unsigned int m_currentState;
	unsigned int m_downStates;
	unsigned int m_upStates;
};

