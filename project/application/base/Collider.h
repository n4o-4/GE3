#pragma once
#include "WorldTransform.h"
class Collider
{
public:

	enum class ColliderType
	{
		typeSphere,
		typeAABB,
	};

public:
	// èâä˙âª
	void Initialize(ColliderType type);

	// çXêV
	void Update();

	ColliderType GetColliderType() { return colliderType; }

	void SetRadius(float radius);

	virtual void OnCollision() {};

	AABB GetAABB() { return aabb; }
	Sphere GetSphere() { return sphere; }

	Vector3 GetWorldPosition() { return { worldTransform.matWorld_.m[3][0],worldTransform.matWorld_.m[3][1],worldTransform.matWorld_.m[3][2] }; }

protected:

	WorldTransform worldTransform;
    
	bool isAlive_ = true;

private:

	ColliderType colliderType;

	AABB aabb;
	Sphere sphere;

	Vector3 radius = {};

	Vector3 worldPosition;

};

