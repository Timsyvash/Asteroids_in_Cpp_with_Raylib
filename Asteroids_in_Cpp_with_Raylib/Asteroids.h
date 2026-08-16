#pragma once
#include <raylib.h>
#include <vector>
#include <string>

using namespace std;

struct Asteroid {
	Rectangle rect;
	float fallSpeed = 0.0f;
	int texIndex = -1;
	float rotation = 0.0f;
	float rotSpeed = 0.0f;
	float velX = 0.0f;
};

class Asteroids {
private:
	Vector2 pos;
	vector<string> paths_asteroids;
	vector<Texture2D> textures_asteroids;
	vector<Asteroid> list;

	Asteroid CreateAsteroidOutside();

	float highestY = 0.0f;
	float defaultWidth = 37.0f;
	float defaultHeight = 35.0f;
	short maxAsteroids = 6;
	short minSpacing = 10;
	short maxSpacing = 40;
public:
	Asteroids();
	~Asteroids();
	void InitTextures();
	void Init(float startX, float startY);
	void Draw();
	void Update(const Vector2& playerPos);
	int GetCount() const { return (int)list.size(); }
	bool CheckCollisionCircle(const Vector2& center, float radius);
	bool CheckCollisionRect(const Rectangle& rect);
};