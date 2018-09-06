#pragma once
#include "Scene.h"
#include "EngineSettings.h"
#include "Display.h"
#include "Keyboard.h"
#include "Mouse.h"

class GameObject;


class Scene {
public:
	virtual void setUp(GameObject* const& root) {}
	virtual void tearDown(GameObject* const& root) {}
};