#include <raylib.h>
#include "SpaceShip.h"
#include "Lasers.h"
#include "Asteroids.h"
#include <string>

int main()
{
    // 1. Спочатку обов'язково відкриваємо вікно!
    InitWindow(800, 600, "Asteroids");
    SetTargetFPS(60);

    // Initialize audio
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "Audio device failed to initialize");
    }

    // 2. Тільки ТЕПЕР створюємо корабель, лазери та астероїди
    SpaceShip ship;
    Lasers laser({ 0, 0 }, 500);
    Asteroids asteroids;

    // Load asteroid textures after InitWindow
    asteroids.InitTextures();
    // Initialize asteroid positions (place relative to ship)
    Vector2 shipPos = { (float)GetScreenWidth() * 0.5f, (float)GetScreenHeight() * 0.5f };
    asteroids.Init(shipPos.x, shipPos.y - 200.0f);

    // Load sounds (optional)
    Sound explosion = {0};
    bool explosionLoaded = false;
    std::string explosionPath = "sounds/explosion.wav";
    if (FileExists(explosionPath.c_str())) {
        explosion = LoadSound(explosionPath.c_str());
        explosionLoaded = true;
    } else {
        TraceLog(LOG_WARNING, "Explosion sound not found: %s", explosionPath.c_str());
    }
    Sound laserSound = {0};
    bool laserLoaded = false;
    std::string laserPath = "sounds/laser.wav";
    if (FileExists(laserPath.c_str())) {
        laserSound = LoadSound(laserPath.c_str());
        laserLoaded = true;
    } else {
        TraceLog(LOG_WARNING, "Laser sound not found: %s", laserPath.c_str());
    }

    bool paused = false;
    bool gameOver = false;

    while (!WindowShouldClose())
    {
        // Input: pause toggle
        if (IsKeyPressed(KEY_P)) paused = !paused;

        if (!paused && !gameOver) {
            // Update
            ship.Update();
            // If player fired this frame, play laser sound
            if (IsKeyPressed(KEY_SPACE) && laserLoaded) PlaySound(laserSound);
            laser.Update(ship.GetPos(), ship.GetRotation());
            asteroids.Update(ship.GetPos());

            // Collision: approximate ship radius
            float shipRadius = 12.0f;
            if (asteroids.CheckCollisionCircle(ship.GetPos(), shipRadius)) {
                // ship collided
                ship.SetAlive(false);
                gameOver = true;
                if (explosionLoaded) PlaySound(explosion);
            }

            // Laser vs asteroid collision
            if (laser.active) {
                Rectangle lr = laser.getRect();
                if (asteroids.CheckCollisionRect(lr)) {
                    laser.active = false;
                    if (explosionLoaded) PlaySound(explosion);
                }
            }
        } else if (gameOver) {
            // allow restart or quit
            if (IsKeyPressed(KEY_R)) {
                ship.Spawn();
                asteroids.Init((float)GetScreenWidth() * 0.5f, (float)GetScreenHeight() * 0.5f - 200.0f);
                gameOver = false;
            }
            if (IsKeyPressed(KEY_Q)) {
                break; // exit loop
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        ship.Draw();
        laser.Draw();
        asteroids.Draw();

        if (paused) {
            DrawText("PAUSED - Press P to resume", GetScreenWidth() / 2 - 140, GetScreenHeight() / 2, 20, RAYWHITE);
        }
        if (gameOver) {
            DrawText("GAME OVER - Press R to restart or Q to quit", GetScreenWidth() / 2 - 220, GetScreenHeight() / 2 - 20, 20, RAYWHITE);
        }

        EndDrawing();
    }

    // Cleanup
    if (explosionLoaded) UnloadSound(explosion);
    if (laserLoaded) UnloadSound(laserSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}