#include "EngineTestPch.h"
#include "ObjectLibrary.h"
#include "Resources\MeshResource.h"
#include "Resources\AnimationResource.h"

TEST(ObjectLibrary, ReturnsAllMeshes_When_GetIsCalledWithMeshType)
{
	uuids::random_generator uuidGen;
	auto& objectLibrary = DerydocaEngine::ObjectLibrary::getInstance();

	auto meshResource = std::make_shared<DerydocaEngine::Resources::MeshResource>();
	meshResource->setType(DerydocaEngine::Resources::ResourceType::MeshResourceType);
	meshResource->setId(uuidGen());
	meshResource->setName("Mesh 1");
	objectLibrary.addResource(meshResource);

	auto animResource = std::make_shared<DerydocaEngine::Resources::AnimationResource>();
	animResource->setType(DerydocaEngine::Resources::ResourceType::AnimationResourceType);
	animResource->setId(uuidGen());
	animResource->setName("Animation 1");
	objectLibrary.addResource(animResource);

	auto meshResources = objectLibrary.getResourcesOfType(DerydocaEngine::Resources::ResourceType::MeshResourceType);

	EXPECT_EQ(1, meshResources.size());
	EXPECT_EQ("Mesh 1", meshResources[0]->getName());
}
