#pragma once

#include "Inspector\InspectorRegistrationMacros.h"
#include "Editor\Inspector\InspectorRendererFactory.h"

#define TEST_INSPECTOR_REGISTRATION(TYPE) \
auto& factory = DerydocaEngine::Editor::Inspector::InspectorRendererFactory::getInstance();\
auto inspector = factory.getInspectorRenderer<TYPE>();\
EXPECT_TRUE(inspector != nullptr);\

