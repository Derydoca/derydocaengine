#include "EditorComponentsPch.h"
#include "SceneHierarchyWindow.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Components
{

	SceneHierarchyWindow::SceneHierarchyWindow() :
		m_sceneRoot()
	{
	}

	SceneHierarchyWindow::~SceneHierarchyWindow()
	{
	}

	void SceneHierarchyWindow::renderWindow()
	{
		// If we do not have a root object, try to get one
		if (m_sceneRoot.expired())
		{
			// Get the active scene
			Scenes::SceneManager& sceneManager = Scenes::SceneManager::getInstance();
			auto scene = sceneManager.getActiveScene();
			if (!scene)
			{
				return;
			}

			// Get the root GameObject for the scene
			m_sceneRoot = scene->getRoot();
			return;
		}

		// Render each GameObject under the root
		for (auto child : m_sceneRoot.lock()->getChildren())
		{
			renderGameObjectTreeNode(child);
		}
	}

	void SceneHierarchyWindow::renderGameObjectTreeNode(std::shared_ptr<GameObject> gameObject)
	{
		if (ImGui::TreeNode(boost::uuids::to_string(gameObject->getId()).c_str(), gameObject->getName().c_str()))
		{
			// render all child GameObjects
			for (auto child : gameObject->getChildren())
			{
				renderGameObjectTreeNode(child);
			}

			ImGui::TreePop();
		}
	}

}