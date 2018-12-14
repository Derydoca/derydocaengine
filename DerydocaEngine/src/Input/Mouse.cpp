#include "EnginePch.h"
#include "Input\Mouse.h"

#include "sdl2\SDL.h"

namespace DerydocaEngine::Input
{

	Mouse::Mouse()
	{
	}

	Mouse::~Mouse()
	{
	}

	void Mouse::init()
	{
		SDL_GetMouseState(&m_currentPos.x, &m_currentPos.y);
		m_prevPos.x = m_currentPos.x;
		m_prevPos.y = m_currentPos.y;
	}

	void Mouse::update()
	{
		m_tick++;

		// Move the last recorded position to the previous position
		m_prevPos.x = m_currentPos.x;
		m_prevPos.y = m_currentPos.y;

		// Query the mouse for it's current position
		unsigned int buttonMask = SDL_GetMouseState(&m_currentPos.x, &m_currentPos.y);

		// Update the mouse's button states
		for (int i = 0; i < 5; i++)
		{
			m_keys[i].setState(buttonMask & SDL_BUTTON(i + 1), m_tick);
		}
	}

	void Mouse::setRelative(bool const & isRelative)
	{
		// Set the mouse to relative mode and flush the first relative move so that the first query isn't incorrect
		SDL_SetRelativeMouseMode((SDL_bool)isRelative);
		if (isRelative) {
			SDL_GetRelativeMouseState(nullptr, nullptr);
		}
	}

	bool Mouse::isKeyDown(int const& keycode) const
	{
		if (keycode < 0 || keycode >= 5) {
			return false;
		}
		return m_keys[keycode].isDown();
	}

	bool Mouse::isKeyDownFrame(int const& keycode) const
	{
		if (keycode < 0 || keycode >= 5) {
			return false;
		}
		Key key = m_keys[keycode];
		return key.isDown() && key.getStateChangeTick() == m_tick;
	}

	bool Mouse::isKeyUpFrame(int const& keycode) const
	{
		if (keycode < 0 || keycode >= 5) {
			return false;
		}
		Key key = m_keys[keycode];
		return !key.isDown() && key.getStateChangeTick() == m_tick;
	}

	glm::ivec2 Mouse::getRelativeMovement() const {
		glm::ivec2 diff;
		SDL_GetRelativeMouseState(&diff.x, &diff.y);
		return diff;
	}

}
