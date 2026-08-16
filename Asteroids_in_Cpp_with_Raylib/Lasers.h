#pragma once
#include <raylib.h>

class Lasers {
private:
	Vector2 position;
	short speed;
	Vector2 direction;
public:
	Lasers(Vector2 position, short speed);
	void Update(const Vector2& shipPos, float shipRotation);
	void Draw();
	Rectangle getRect();
	bool active;
};