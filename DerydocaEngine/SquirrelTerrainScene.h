#pragma once
#include "Scene.h"
#include "EngineSettings.h"
#include "Display.h"
#include "Keyboard.h"
#include "Mouse.h"

class GameObject;

class SquirrelTerrainScene : public Scene
{
public:
	SquirrelTerrainScene();
	~SquirrelTerrainScene();

	virtual void setUp(GameObject* root, EngineSettings* settings, Display* display, Keyboard* keyboard, Mouse* mouse);
	virtual void tearDown(GameObject* root);
};
