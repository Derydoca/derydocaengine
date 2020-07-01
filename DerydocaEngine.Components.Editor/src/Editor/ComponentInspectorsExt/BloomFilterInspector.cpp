#include "EditorComponentsPch.h"
#include "BloomFilterInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::BloomFilterInspector::render(std::shared_ptr<Object> object)
{
	auto bloomFilter = std::static_pointer_cast<Ext::BloomFilter>(object);
	ImGui::SliderFloat("Luminosity Threshold", &bloomFilter->GetThreshold(), 0.0f, 1.0f);
}
