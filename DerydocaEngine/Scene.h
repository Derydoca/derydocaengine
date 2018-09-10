#pragma once

class GameObject;

class Scene {
public:
	virtual ~Scene() {}
	virtual void setUp(GameObject* const& root) {}
	virtual void tearDown(GameObject* const& root) {}
};