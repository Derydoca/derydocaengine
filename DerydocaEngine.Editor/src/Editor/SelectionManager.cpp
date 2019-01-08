#include "EditorPch.h"
#include "SelectionManager.h"

namespace DerydocaEngine::Editor
{

	SelectionManager::SelectionManager() :
		m_selectionGroups(MAX_SELECTION_GROUPS)
	{
		for (int i = 0; i < MAX_SELECTION_GROUPS; i++)
		{
			m_selectionGroups[0] = std::make_shared<SelectionGroup>();
		}
	}

	SelectionManager::~SelectionManager()
	{
	}

	std::shared_ptr<SelectionGroup> DerydocaEngine::Editor::SelectionManager::getPrimarySelectionGroup() const
	{
		return m_selectionGroups[0];
	}

	std::shared_ptr<SelectionGroup> DerydocaEngine::Editor::SelectionManager::getSelectionGroup(int groupId) const
	{
		return m_selectionGroups.at(groupId);
	}

}
