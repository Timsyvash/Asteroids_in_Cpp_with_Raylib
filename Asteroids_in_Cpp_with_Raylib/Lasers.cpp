#include "Lasers.h"
#include <cmath>

Lasers::Lasers(Vector2 position, short speed)
{
    this->position = position;
    this->speed = speed;
    direction = { 0.0f, 0.0f };
    active = false;
}

void Lasers::Update(const Vector2& shipPos, float shipRotation)
{
    float dt = GetFrameTime();

    // Постріл на Space
    if (IsKeyPressed(KEY_SPACE) && !active) {
        position = shipPos;
        float radians = shipRotation * DEG2RAD;

        // Поворот вгору (0 градусів): X = sin, Y = -cos
        direction = { sinf(radians), -cosf(radians) };
        active = true;
    }

    if (active) {
        position.x += direction.x * speed * dt;
        position.y += direction.y * speed * dt;

        // Вихід за межі екрана
        if (position.x < 0 || position.x > GetScreenWidth() ||
            position.y < 0 || position.y > GetScreenHeight()) {
            active = false;
        }
    }
}

void Lasers::Draw()
{
    if (active) {
        DrawCircleV(position, 3.0f, RED);
    }
}

Rectangle Lasers::getRect()
{
    return { position.x - 2.5f, position.y - 2.5f, 5.0f, 5.0f };
}