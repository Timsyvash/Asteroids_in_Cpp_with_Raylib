#pragma once
#include <raylib.h>
#include "Settings.h"
#include "Asteroids.h"

class Game {
public:
	Game();
	~Game();
	void Run();

private:
	Asteroids asteroids;
	Vector2 playerPos;
	Texture2D playerTexture = { 0 };
	float playerRotation = 0.0f;

	void Update();
	void Draw();
};
