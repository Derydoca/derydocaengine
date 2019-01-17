#include "EditorTestPch.h"

#include "Inspector\InspectorRegistrationMacros.h"

#include "Components\Camera.h"
#include "Components\FrameStats.h"
#include "Components\KeyboardMover.h"
#include "Components\Light.h"
#include "Components\MaterialRefresher.h"
#include "Components\MeshRenderer.h"
#include "Components\Rotator.h"
#include "Components\ScreenshotUtil.h"
#include "Components\SkinnedMeshRenderer.h"
#include "Components\SpriteRenderer.h"
#include "Components\Terrain.h"
#include "Components\TextRenderer.h"
#include "Components\Transform.h"
#include "Components\WasdMover.h"

TEST(InspectorRegistration_Component, Camera) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::Camera);
}

TEST(InspectorRegistration_Component, FrameStats) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::FrameStats);
}

TEST(InspectorRegistration_Component, KeyboardMover) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::KeyboardMover);
}

TEST(InspectorRegistration_Component, Light) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::Light);
}

TEST(InspectorRegistration_Component, MaterialRefresher) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::MaterialRefresher);
}

TEST(InspectorRegistration_Component, MeshRenderer) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::MeshRenderer);
}

TEST(InspectorRegistration_Component, Rotator) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::Rotator);
}

TEST(InspectorRegistration_Component, ScreenshotUtil) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::ScreenshotUtil);
}

TEST(InspectorRegistration_Component, SkinnedMeshRenderer) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::SkinnedMeshRenderer);
}

TEST(InspectorRegistration_Component, SpriteRenderer) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::SpriteRenderer);
}

TEST(InspectorRegistration_Component, Terrain) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::Terrain);
}

TEST(InspectorRegistration_Component, TextRenderer) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::TextRenderer);
}

TEST(InspectorRegistration_Component, Transform) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::Transform);
}

TEST(InspectorRegistration_Component, WasdMover) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Components::WasdMover);
}
