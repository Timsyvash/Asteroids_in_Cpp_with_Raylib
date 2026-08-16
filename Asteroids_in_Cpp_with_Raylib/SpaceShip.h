#pragma once
#include <raylib.h>

class SpaceShip {
private:
	Texture2D texture;
	Vector2 pos;
	Vector2 velocity = { 0.0f, 0.0f };
	float rotation = 0.0f;        // Кут повороту в градусах
	float speed = 350.0f;         // Сила тяги двигуна
	float rotationSpeed = 220.0f; // Швидкість повороту (град/сек)
	float friction = 0.985f;      // Інерційне гальмування у космосі
	bool alive = true;

public:
	SpaceShip();
	~SpaceShip();

	void Spawn();
	void Update();
	void Draw() const;

	Vector2 GetPos() const { return pos; }
	float GetRotation() const { return rotation; }
	float GetRadius() const; // collision radius (half of sprite size or default)
	bool IsAlive() const { return alive; }
	void SetAlive(bool a) { alive = a; }
};