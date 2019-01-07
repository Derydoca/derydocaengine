#include "EditorPch.h"
#include "SelectionManager.h"

namespace DerydocaEngine::Editor
{

	SelectionManager::SelectionManager() :
		m_selectionGroups(MAX_SELECTION_GROUPS)
	{
	}

	SelectionManager::~SelectionManager()
	{
	}

	SelectionGroup DerydocaEngine::Editor::SelectionManager::getPrimarySelectionGroup() const
	{
		return m_selectionGroups[0];
	}

	SelectionGroup DerydocaEngine::Editor::SelectionManager::getSelectionGroup(int groupId) const
	{
		return m_selectionGroups.at(groupId);
	}

}
