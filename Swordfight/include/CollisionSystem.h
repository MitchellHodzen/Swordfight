#pragma once

struct Rect;
struct Vector2;

class CollisionSystem
{
public:
	CollisionSystem();
	~CollisionSystem();
	void CheckCollisions();
private:
	bool RectsColliding(Rect& rect1, Vector2& pos1, Rect& rect2, Vector2& pos2);

};

