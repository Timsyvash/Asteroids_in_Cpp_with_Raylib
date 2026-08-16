#include "Asteroids.h"
#include <random>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cctype>

Asteroids::Asteroids()
{
	pos = { 0.0f, 0.0f };
	// default fallback paths (kept for compatibility)
	paths_asteroids = {
		"images/asteroid_1.png",
		"images/asteroid_2.png",
		"images/asteroid_3.png",
		"images/asteroid_4.png"
	};
}

bool Asteroids::CheckCollisionRect(const Rectangle& rect)
{
	for (size_t i = 0; i < list.size(); ++i) {
		const Asteroid &a = list[i];
		if (CheckCollisionRecs(a.rect, rect)) {
			// respawn this asteroid
			list[i] = CreateAsteroidOutside();
			return true;
		}
	}
	return false;
}

bool Asteroids::CheckCollisionCircle(const Vector2& center, float radius)
{
	for (size_t i = 0; i < list.size(); ++i) {
		const Asteroid &a = list[i];
		// approximate asteroid by circle using max(width,height)/2
		float ax = a.rect.x + a.rect.width * 0.5f;
		float ay = a.rect.y + a.rect.height * 0.5f;
		float ar = (a.rect.width > a.rect.height) ? a.rect.width * 0.5f : a.rect.height * 0.5f;
		float dx = center.x - ax;
		float dy = center.y - ay;
		float dist2 = dx*dx + dy*dy;
		float rsum = radius + ar;
		if (dist2 <= rsum * rsum) {
			// respawn this asteroid
			list[i] = CreateAsteroidOutside();
			return true;
		}
	}
	return false;
}

void Asteroids::InitTextures()
{
	textures_asteroids.clear();
	namespace fs = std::filesystem;
	// Prefer a dedicated asteroids folder to avoid loading player/other sprites
	const fs::path asteroidsDir = "images/asteroids";
	const fs::path imagesDir = "images";
	if (fs::exists(asteroidsDir) && fs::is_directory(asteroidsDir)) {
		for (const auto &entry : fs::directory_iterator(asteroidsDir)) {
			if (!entry.is_regular_file()) continue;
			std::string ext = entry.path().extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp") {
				std::string fullPath = entry.path().string();
				Image img = LoadImage(fullPath.c_str());
				if (img.data != nullptr) {
					Texture2D t = LoadTextureFromImage(img);
					UnloadImage(img);
					if (t.id != 0) textures_asteroids.push_back(t);
				}
			}
		}
	} else if (fs::exists(imagesDir) && fs::is_directory(imagesDir)) {
		// images/ exists but no images/asteroids/ folder: load only files containing "asteroid" in the name
		for (const auto &entry : fs::directory_iterator(imagesDir)) {
			if (!entry.is_regular_file()) continue;
			std::string filename = entry.path().filename().string();
			std::string filenameLower = filename;
			std::transform(filenameLower.begin(), filenameLower.end(), filenameLower.begin(), [](unsigned char c){ return std::tolower(c); });
			if (filenameLower.find("asteroid") == std::string::npos) continue; // skip non-asteroid images
			std::string ext = entry.path().extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp") {
				std::string fullPath = entry.path().string();
				Image img = LoadImage(fullPath.c_str());
				if (img.data != nullptr) {
					Texture2D t = LoadTextureFromImage(img);
					UnloadImage(img);
					if (t.id != 0) textures_asteroids.push_back(t);
				}
			}
		}
	} else {
		// fallback to paths_asteroids
		for (const auto &p : paths_asteroids) {
			Image img = LoadImage(p.c_str());
			if (img.data != nullptr) {
				Texture2D t = LoadTextureFromImage(img);
				UnloadImage(img);
				if (t.id != 0) textures_asteroids.push_back(t);
			}
		}
	}
}

Asteroids::~Asteroids()
{
	for (auto &t : textures_asteroids) {
		if (t.id != 0) UnloadTexture(t);
	}
}

void Asteroids::Init(float startX, float startY)
{
	list.clear();
	highestY = startY;

	// spawn a few asteroids inside screen so player sees them
	int spawnInside = std::max(1, maxAsteroids / 2);
	for (int i = 0; i < maxAsteroids; ++i) {
		Asteroid a;
		if (i < spawnInside) {
			int tex = textures_asteroids.empty() ? -1 : GetRandomValue(0, (int)textures_asteroids.size() - 1);
			float w = (tex >= 0) ? (float)textures_asteroids[tex].width : defaultWidth;
			float h = (tex >= 0) ? (float)textures_asteroids[tex].height : defaultHeight;
			a.texIndex = tex;
			a.rect.width = w;
			a.rect.height = h;
			a.rect.x = (float)GetRandomValue(0, GetScreenWidth() - (int)w);
			a.rect.y = (float)GetRandomValue(0, GetScreenHeight() - (int)h);
			a.velX = (float)GetRandomValue(-60, 60);
			a.fallSpeed = (float)GetRandomValue(-40, 40);
			a.rotation = (float)GetRandomValue(0, 360);
			a.rotSpeed = (float)GetRandomValue(-90, 90);
		} else {
			a = CreateAsteroidOutside();
		}
		list.push_back(a);
	}
}

Asteroid Asteroids::CreateAsteroidOutside()
{
	Asteroid a;
	int tex = textures_asteroids.empty() ? -1 : GetRandomValue(0, (int)textures_asteroids.size() - 1);
	float w = (tex >= 0) ? (float)textures_asteroids[tex].width : defaultWidth;
	float h = (tex >= 0) ? (float)textures_asteroids[tex].height : defaultHeight;

	int side = GetRandomValue(0, 3);
	float x = 0.0f, y = 0.0f;
	float velX = 0.0f;
	float fall = 0.0f;

	switch (side) {
	case 0:
		x = -w - (float)GetRandomValue(10, 100);
		y = (float)GetRandomValue(-100, GetScreenHeight() + 100);
		velX = (float)GetRandomValue(30, 120);
		fall = (float)GetRandomValue(-20, 50);
		break;
	case 1:
		x = (float)GetScreenWidth() + (float)GetRandomValue(10, 100);
		y = (float)GetRandomValue(-100, GetScreenHeight() + 100);
		velX = -(float)GetRandomValue(30, 120);
		fall = (float)GetRandomValue(-20, 50);
		break;
	case 2:
		x = (float)GetRandomValue(-100, GetScreenWidth() + 100);
		y = -h - (float)GetRandomValue(10, 100);
		velX = (float)GetRandomValue(-50, 50);
		fall = (float)GetRandomValue(30, 150);
		break;
	default:
		x = (float)GetRandomValue(-100, GetScreenWidth() + 100);
		y = (float)GetScreenHeight() + (float)GetRandomValue(10, 100);
		velX = (float)GetRandomValue(-50, 50);
		fall = -(float)GetRandomValue(30, 150);
		break;
	}

	a.rect = { x, y, w, h };
	a.texIndex = tex;
	a.fallSpeed = fall;
	a.rotation = (float)GetRandomValue(0, 360);
	a.rotSpeed = (float)GetRandomValue(-90, 90);
	a.velX = velX;

	return a;
}

void Asteroids::Update(const Vector2& playerPos)
{
	float dt = GetFrameTime();
	for (auto &p : list) {
		p.rect.y += p.fallSpeed * dt;
		p.rotation += p.rotSpeed * dt;
		p.rect.x += p.velX * dt;
	}

	const float margin = 200.0f;
	for (auto &p : list) {
		bool outside = false;
		if (p.rect.x > GetScreenWidth() + margin) outside = true;
		if (p.rect.x + p.rect.width < -margin) outside = true;
		if (p.rect.y > GetScreenHeight() + margin) outside = true;
		if (p.rect.y + p.rect.height < -margin) outside = true;

		if (outside) p = CreateAsteroidOutside();
	}
}

void Asteroids::Draw()
{
	for (size_t i = 0; i < list.size(); ++i) {
		const Asteroid &as = list[i];
		if (as.texIndex >= 0 && as.texIndex < (int)textures_asteroids.size() && textures_asteroids[as.texIndex].id > 0) {
			Texture2D &tex = textures_asteroids[as.texIndex];
			Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
			Rectangle dest = { as.rect.x + as.rect.width * 0.5f, as.rect.y + as.rect.height * 0.5f, as.rect.width, as.rect.height };
			Vector2 origin = { as.rect.width * 0.5f, as.rect.height * 0.5f };
			DrawTexturePro(tex, src, dest, origin, as.rotation, WHITE);
		} else {
			DrawRectangleRec(as.rect, GREEN);
		}
	}
}
