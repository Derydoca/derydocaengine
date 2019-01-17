#include "pch.h"
#include "Inspector\InspectorRegistrationMacros.h"
#include "Resources\AnimationResource.h"
#include "Resources\CubemapResource.h"
#include "Resources\FontResource.h"
#include "Resources\LevelResource.h"
#include "Resources\MaterialResource.h"
#include "Resources\MeshResource.h"
#include "Resources\RasterFontResource.h"
#include "Resources\ShaderResource.h"
#include "Resources\SkeletonResource.h"
#include "Resources\SpriteSheetResource.h"
#include "Resources\TextureResource.h"

TEST(InspectorRegistration_Resource, AnimationResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::AnimationResource);
}

TEST(InspectorRegistration_Resource, CubemapResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::CubemapResource);
}

TEST(InspectorRegistration_Resource, FontResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::FontResource);
}

TEST(InspectorRegistration_Resource, LevelResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::LevelResource);
}

TEST(InspectorRegistration_Resource, MaterialResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::MaterialResource);
}

TEST(InspectorRegistration_Resource, MeshResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::MeshResource);
}

TEST(InspectorRegistration_Resource, RasterFontResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::RasterFontResource);
}

TEST(InspectorRegistration_Resource, ShaderResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::ShaderResource);
}

TEST(InspectorRegistration_Resource, SkeletonResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::SkeletonResource);
}

TEST(InspectorRegistration_Resource, SpriteSheetResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::SpriteSheetResource);
}

TEST(InspectorRegistration_Resource, TextureResource) {
	TEST_INSPECTOR_REGISTRATION(DerydocaEngine::Resources::TextureResource);
}
