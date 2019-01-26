#pragma once

namespace DerydocaEngine::Editor
{

	class EditorGUI
	{
	public:
		static EditorGUI& getInstance()
		{
			static EditorGUI instance = EditorGUI();
			return instance;
		}

		void render();

	private:
		EditorGUI();
		~EditorGUI();
		EditorGUI(const EditorGUI&);

	};

}
