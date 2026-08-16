#include "Game.h"
#include <cmath>

Game::Game()
{
	InitWindow(settings::window_width, settings::window_height, "Asteroids");
	SetTargetFPS(settings::fps);

	// Load textures now that the window and RLGL/OpenGL are initialized
	asteroids.InitTextures();

	// Load player texture (ensure images/player.png exists)
	playerTexture = LoadTexture("images/space_ship.png");

	playerPos = {(float)settings::window_width / 2.0f, (float)settings::window_height / 2.0f};
	asteroids.Init(playerPos.x, playerPos.y - 200.0f);
}

Game::~Game()
{
	if (playerTexture.id != 0) UnloadTexture(playerTexture);
	CloseWindow();
}

void Game::Run()
{
	while (!WindowShouldClose()) {
		Update();

		BeginDrawing();
		ClearBackground(BLACK);
		Draw();
		EndDrawing();
	}
}

void Game::Update()
{
	float dt = GetFrameTime();

	// Simple player movement (arrow keys)
	if (IsKeyDown(KEY_RIGHT)) { playerPos.x += 200.0f * dt; playerRotation = 0.0f; }
	else if (IsKeyDown(KEY_LEFT)) { playerPos.x -= 200.0f * dt; playerRotation = 180.0f; }
	else if (IsKeyDown(KEY_UP)) { playerPos.y -= 200.0f * dt; playerRotation = -90.0f; }
	else if (IsKeyDown(KEY_DOWN)) { playerPos.y += 200.0f * dt; playerRotation = 90.0f; }

	// Keep player inside screen
	if (playerPos.x < 0) playerPos.x = 0;
	if (playerPos.y < 0) playerPos.y = 0;
	if (playerPos.x > GetScreenWidth()) playerPos.x = (float)GetScreenWidth();
	if (playerPos.y > GetScreenHeight()) playerPos.y = (float)GetScreenHeight();

	asteroids.Update(playerPos);
}

void Game::Draw()
{
	asteroids.Draw();

	// Draw player: prefer loaded texture, fallback to procedural triangle
	if (playerTexture.id > 0) {
		// Draw the texture with preserved aspect ratio, fitting into a displaySize box
		const float displaySize = 48.0f; // max dimension to fit the texture into
		Rectangle src = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };
		float texW = (float)playerTexture.width;
		float texH = (float)playerTexture.height;
		float scale = displaySize / ((texW > texH) ? texW : texH);
		Rectangle dest = { playerPos.x, playerPos.y, texW * scale, texH * scale };
		Vector2 origin = { dest.width * 0.5f, dest.height * 0.5f };
		DrawTexturePro(playerTexture, src, dest, origin, playerRotation, WHITE);
	} else {
		float s = 16.0f; // half-height of the triangle
		Vector2 v1 = { 0.0f, -s };
		Vector2 v2 = { -s * 0.6f, s };
		Vector2 v3 = { s * 0.6f, s };
		float rad = playerRotation * DEG2RAD;
		float cs = cosf(rad);
		float sn = sinf(rad);
		Vector2 rv1 = { playerPos.x + v1.x * cs - v1.y * sn, playerPos.y + v1.x * sn + v1.y * cs };
		Vector2 rv2 = { playerPos.x + v2.x * cs - v2.y * sn, playerPos.y + v2.x * sn + v2.y * cs };
		Vector2 rv3 = { playerPos.x + v3.x * cs - v3.y * sn, playerPos.y + v3.x * sn + v3.y * cs };
		DrawTriangle(rv1, rv2, rv3, WHITE);
		DrawTriangleLines(rv1, rv2, rv3, BLACK);
	}
}
