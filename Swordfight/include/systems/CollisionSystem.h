#pragma once

struct BoxCollider;
struct Vector2;

class CollisionSystem
{
public:
	CollisionSystem();
	~CollisionSystem();
	void CheckCollisions();
private:
	bool RectsColliding(BoxCollider& col1, Vector2& pos1, BoxCollider& col2, Vector2& pos2);

};

