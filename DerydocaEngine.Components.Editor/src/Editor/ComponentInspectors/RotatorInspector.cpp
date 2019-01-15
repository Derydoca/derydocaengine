#include "EditorComponentsPch.h"
#include "RotatorInspector.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	void RotatorInspector::render(std::shared_ptr<Object> object)
	{
		auto rotator = std::static_pointer_cast<Components::Rotator>(object);

		ImGui::DragFloat("Speed", &rotator->getSpeed());
	}

}
