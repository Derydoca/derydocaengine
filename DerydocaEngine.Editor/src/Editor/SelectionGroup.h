#pragma once

namespace DerydocaEngine::Editor
{

	class SelectionGroup
	{
	public:
		void select(boost::uuids::uuid id)
		{
			m_selection.clear();
			m_selection.push_back(id);
		}

		void select(std::list<boost::uuids::uuid> ids)
		{
			m_selection = ids;
		}

		bool isSelected(boost::uuids::uuid id) const
		{
			for (auto selectedId : m_selection)
			{
				if (id == selectedId)
				{
					return true;
				}
			}
			return false;
		}

		std::list<boost::uuids::uuid> getSelection() const { return m_selection; }

	private:
		std::list<boost::uuids::uuid> m_selection;

	};

}