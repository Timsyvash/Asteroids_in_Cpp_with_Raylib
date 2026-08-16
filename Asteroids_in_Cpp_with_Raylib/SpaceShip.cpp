#include "SpaceShip.h"
#include <cmath>

SpaceShip::SpaceShip()
{
	Image img = LoadImage("images/space_ship.png");
	texture = LoadTextureFromImage(img);
	UnloadImage(img);
	Spawn();
}

SpaceShip::~SpaceShip()
{
	UnloadTexture(texture);
}

void SpaceShip::Spawn()
{
	// Спавн суворо по центру екрана
	pos = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
	velocity = { 0.0f, 0.0f };
	rotation = 0.0f;
	alive = true;
}

void SpaceShip::Update()
{
	if (!alive) return;

	float dt = GetFrameTime();

	// 1. Поворот корабля (A/D або Стрілки)
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
		rotation -= rotationSpeed * dt;
	}
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		rotation += rotationSpeed * dt;
	}

	// 2. Додавання тяги вперед за напрямком носа (W або Стрілка вгору)
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
		float rad = rotation * DEG2RAD; // Перевід у радіани для sin/cos
		velocity.x += sinf(rad) * speed * dt;
		velocity.y -= cosf(rad) * speed * dt;
	}

	// 3. Рух за інерцією та поступове згасання швидкості
	pos.x += velocity.x * dt;
	pos.y += velocity.y * dt;
	velocity.x *= friction;
	velocity.y *= friction;

	// 4. Телепортація при вильоті за межі екрана (Screen Wrap)
	float radius = (float)texture.width / 2.0f;
	if (pos.x > GetScreenWidth() + radius) pos.x = -radius;
	if (pos.x < -radius) pos.x = (float)GetScreenWidth() + radius;
	if (pos.y > GetScreenHeight() + radius) pos.y = -radius;
	if (pos.y < -radius) pos.y = (float)GetScreenHeight() + radius;
}

void SpaceShip::Draw() const
{
	if (!alive) return;

	Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle destRec = { pos.x, pos.y, (float)texture.width, (float)texture.height };
	Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

	DrawTexturePro(texture, sourceRec, destRec, origin, rotation, WHITE);
}