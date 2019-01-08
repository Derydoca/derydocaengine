#pragma once
#include "SelectionGroup.h"

namespace DerydocaEngine::Editor
{

	const int MAX_SELECTION_GROUPS = 16;

	class SelectionManager
	{
	public:
		static SelectionManager& getInstance()
		{
			static SelectionManager m_instance;
			return m_instance;
		}

		std::shared_ptr<SelectionGroup> getPrimarySelectionGroup() const;
		std::shared_ptr<SelectionGroup> getSelectionGroup(int groupId) const;

	private:
		SelectionManager();
		~SelectionManager();
		SelectionManager(const SelectionManager&);

		std::vector<std::shared_ptr<SelectionGroup>> m_selectionGroups;

	};

}