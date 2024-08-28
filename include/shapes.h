#pragma once

#include "header.h"

struct Obstacle
{
	Rectangle envObjects{};
	Vector2 origin;
	float rotation;
	Color color;

	void Init(Rectangle envObjects, Vector2 origin,
		float rotation, Color color);
	void Draw() const;
	void Update(); // This might not be necessary unless individual updates are needed
};


struct Player
{
    Vector2 center{};
    float radius;
    Color color;

    float speed = 0;
    bool isFloating = true;
    bool onGround = false;
    bool jumpRequested = false;
    const float G = 0.5f; // Gravity constant
    const float jumpVelocity = -10.0f; // Initial velocity for jumping

    void Init(Vector2 center, float radius, Color color);
    void Draw();
    void Update(const std::vector<Obstacle>& obstacles);
};
