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

	void Initialize(ColliderType type);
	void Update();

	ColliderType GetColliderType() { return colliderType; }

	void SetRadius(float radius);

	virtual void OnCollision() {};

	AABB GetAABB() { return aabb; }
	Sphere GetSphere() { return sphere; }

protected:

	WorldTransform worldTransform;
    
	bool isAlive_ = true;

private:

	ColliderType colliderType;

	AABB aabb;
	Sphere sphere;

	Vector3 radius = {};

};

