#pragma once
#include <glm/vec2.hpp>
#include "Input\Key.h"

namespace DerydocaEngine::Input
{

	class Mouse
	{
	public:
		Mouse();
		~Mouse();
		
		Key* getKey(int const& keyIndex) { return &m_keys[keyIndex]; }
		glm::ivec2 getRelativeMovement() const;
		void init();
		bool isKeyDown(int const& keycode) const;
		bool isKeyDownFrame(int const& keycode) const;
		bool isKeyUpFrame(int const& keycode) const;
		void setRelative(bool const& isRelative);
		void setRelativeWheelMovement(const int x, const int y)
		{
			m_relativeMouseWheelMovement.x = x;
			m_relativeMouseWheelMovement.y = y;
		}
		const int getRelativeWheelMovementX() const { return m_relativeMouseWheelMovement.x; }
		const int getRelativeWheelMovementY() const { return m_relativeMouseWheelMovement.y; }
		void update();
	private:
		glm::ivec2 m_currentPos;
		glm::ivec2 m_prevPos;
		glm::ivec2 m_relativeMouseWheelMovement;
		unsigned long m_tick;
		Key m_keys[5];
	};

}
