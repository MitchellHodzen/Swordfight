#pragma once

struct HorizontalCollider;
struct Vector2;
struct Transform;

class CollisionSystem
{
public:
	CollisionSystem();
	~CollisionSystem();
	void CheckCollisions();
private:
	bool HorizontalColliding(HorizontalCollider& col1, Vector2& pos1, HorizontalCollider& col2, Vector2& pos2);
	Vector2 GetParentModifiedPosition(Transform& transform);

};

