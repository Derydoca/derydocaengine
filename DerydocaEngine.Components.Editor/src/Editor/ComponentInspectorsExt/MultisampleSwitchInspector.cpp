#include "EditorComponentsPch.h"
#include "MultisampleSwitchInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::MultisampleSwitchInspector::render(std::shared_ptr<Object> object)
{
	auto multisampleSwitch = std::static_pointer_cast<Ext::MultisampleSwitch>(object);

	bool isOn = multisampleSwitch->isMultisampleOn();
	if (ImGui::Checkbox("Enable Multisampling", &isOn))
	{
		multisampleSwitch->setEnableMultisample(isOn);
	}
}
