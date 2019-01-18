#include "EditorTestPch.h"

#include "Inspector\InspectorRegistrationMacros.h"

#include "ComponentsExt\BezierCurveRenderer.h"
#include "ComponentsExt\BloomFilter.h"
#include "ComponentsExt\EdgeDetectionFilter.h"
#include "ComponentsExt\GammaCorrectionFilter.h"
#include "ComponentsExt\GaussianBlurFilter.h"
#include "ComponentsExt\ImageProjector.h"
#include "ComponentsExt\MultisampleSwitch.h"
#include "ComponentsExt\NightVisionFilter.h"
#include "ComponentsExt\NoiseTexture.h"
#include "ComponentsExt\ParticleContinuousFountain.h"
#include "ComponentsExt\ParticleFountain.h"
#include "ComponentsExt\ParticleInstanced.h"
#include "ComponentsExt\ParticleSystem.h"
#include "ComponentsExt\ShaderSubroutineSwitcher.h"
#include "ComponentsExt\TessellatedMeshRenderer.h"
#include "ComponentsExt\TessellatingQuad.h"
#include "ComponentsExt\WaveDisplacement.h"
#include "ComponentsExt\WoodSliceTexture.h"

TEST(InspectorRegistration_Component_Ext, BezierCurveRenderer) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::BezierCurveRenderer);
}

TEST(InspectorRegistration_Component_Ext, BloomFilter) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::BloomFilter);
}

TEST(InspectorRegistration_Component_Ext, EdgeDetectionFilter) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::EdgeDetectionFilter);
}

TEST(InspectorRegistration_Component_Ext, GammaCorrectionFilter) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::GammaCorrectionFilter);
}

TEST(InspectorRegistration_Component_Ext, GaussianBlurFilter) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::GaussianBlurFilter);
}

TEST(InspectorRegistration_Component_Ext, ImageProjector) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::ImageProjector);
}

TEST(InspectorRegistration_Component_Ext, MultisampleSwitch) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::MultisampleSwitch);
}

TEST(InspectorRegistration_Component_Ext, NightVisionFilter) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::NightVisionFilter);
}

TEST(InspectorRegistration_Component_Ext, NoiseTexture) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::NoiseTexture);
}

TEST(InspectorRegistration_Component_Ext, ParticleContinuousFountain) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::ParticleContinuousFountain);
}

TEST(InspectorRegistration_Component_Ext, ParticleFountain) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::ParticleFountain);
}

TEST(InspectorRegistration_Component_Ext, ParticleInstanced) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::ParticleInstanced);
}

TEST(InspectorRegistration_Component_Ext, ParticleSystem) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::ParticleSystem);
}

TEST(InspectorRegistration_Component_Ext, ShaderSubroutineSwitcher) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::ShaderSubroutineSwitcher);
}

TEST(InspectorRegistration_Component_Ext, TessellatedMeshRenderer) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::TessellatedMeshRenderer);
}

TEST(InspectorRegistration_Component_Ext, TessellatingQuad) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::TessellatingQuad);
}

TEST(InspectorRegistration_Component_Ext, WaveDisplacement) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::WaveDisplacement);
}

TEST(InspectorRegistration_Component_Ext, WoodSliceTexture) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Ext::WoodSliceTexture);
}
