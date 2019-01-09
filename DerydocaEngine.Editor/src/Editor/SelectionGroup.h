#pragma once
#include "Object.h"

namespace DerydocaEngine::Editor
{

	class SelectionGroup
	{
	public:
		SelectionGroup() :
			m_selection()
		{}

		void select(const std::weak_ptr<Object> selection)
		{
			m_selection.clear();
			m_selection.push_back(selection.lock());
		}

		bool isSelected(std::shared_ptr<Object> object) const
		{
			for (auto selectedObject : m_selection)
			{
				if (object == selectedObject)
				{
					return true;
				}
			}
			return false;
		}

		std::vector<std::shared_ptr<Object>> getSelection() const { return m_selection; }

	private:
		std::vector<std::shared_ptr<Object>> m_selection;

	};

}