#include "EditorComponentsPch.h"
#include "SceneHierarchyWindow.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Components
{

	SceneHierarchyWindow::SceneHierarchyWindow() :
		m_SceneRoot()
	{
		m_SelectionGroup = Editor::SelectionManager::getInstance().getPrimarySelectionGroup();
	}

	SceneHierarchyWindow::~SceneHierarchyWindow()
	{
	}

	void SceneHierarchyWindow::renderWindow()
	{
		// If we do not have a root object, try to get one
		if (m_SceneRoot.expired())
		{
			// Get the active scene
			Scenes::SceneManager& sceneManager = Scenes::SceneManager::getInstance();
			auto scene = sceneManager.getActiveScene();
			if (!scene)
			{
				return;
			}

			// Get the root GameObject for the scene
			m_SceneRoot = scene->getRoot();
			return;
		}

		// Render each GameObject under the root
		for (auto child : m_SceneRoot.lock()->getChildren())
		{
			renderGameObjectTreeNode(child);
		}
	}

	void SceneHierarchyWindow::renderGameObjectTreeNode(std::shared_ptr<GameObject> gameObject)
	{
		// Get the string representation of the game object ID
		auto id = gameObject->getId().to_string();
		
		// Set render flags for this game object
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow;
		if (m_SelectionGroup->isSelected(gameObject))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		if (gameObject->getChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		// Render the element
		bool isExpanded = ImGui::TreeNodeEx(id.c_str(), flags, gameObject->getName().c_str());

		// Select the element if it was clicked
		if (ImGui::IsItemClicked())
		{
			m_SelectionGroup->select(gameObject);
		}

		// If it was expanded, render the child elements
		if (isExpanded)
		{
			ImGui::TreePush();

			for (auto child : gameObject->getChildren())
			{
				renderGameObjectTreeNode(child);
			}

			ImGui::TreePop();
		}

	}

}