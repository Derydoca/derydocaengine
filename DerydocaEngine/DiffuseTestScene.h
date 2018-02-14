#pragma once
#include "Scene.h"
#include "EngineSettings.h"
#include "Display.h"
#include "Keyboard.h"
#include "Mouse.h"

class GameObject;

class DiffuseTestScene : public Scene
{
public:
	DiffuseTestScene();
	~DiffuseTestScene();

	virtual void setUp(GameObject* root);
	virtual void tearDown(GameObject* root);
};

