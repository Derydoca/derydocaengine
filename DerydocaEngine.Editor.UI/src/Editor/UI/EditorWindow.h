#pragma once
#include <string>
#include "GameObject.h"

namespace DerydocaEngine::Editor::UI
{

	class EditorWindow
	{
	public:
		int Run(
			const std::string& projectPath,
			const std::string& editorComponentsLevelIdentifier,
			const std::string& levelIdentifier
		);
	private:
		std::shared_ptr<Scenes::SerializedScene> SetUpLevelObject(
			const std::string& levelIdentifier,

			const std::string& levelType,
			const std::shared_ptr<GameObject> rootSceneObject
		);
	};

}
