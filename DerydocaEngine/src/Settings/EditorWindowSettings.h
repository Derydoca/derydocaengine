#pragma once

namespace DerydocaEngine::Settings
{
	enum WindowState
	{
		Normal,
		Maximized,
		FullScreen
	};

	class EditorWindowSettings
	{
	public:
		EditorWindowSettings() :
			m_position(-1, -1),
			m_size(800, 600),
			m_state(Normal)
		{
		}

		const int2 getPosition() const { return m_position; }
		const int2 getSize() const { return m_size; }
		const WindowState getState() const { return m_state; }

		void setPosition(int2 position) { m_position = position; }
		void setSize(int2 size) { m_size = size; }
		void setState(WindowState state) { m_state = state; }

		SERIALIZE_FUNC(
			SERIALIZE(m_position),
			SERIALIZE(m_size),
			SERIALIZE(m_state)
		);
	private:
		int2 m_position;
		int2 m_size;
		WindowState m_state;
	};
}