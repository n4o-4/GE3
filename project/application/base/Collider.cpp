#include "Collider.h"

void Collider::Initialize(ColliderType type)
{
	worldTransform.Initialize();

	colliderType = type;

	if (colliderType == ColliderType::typeAABB)
	{
		aabb.min = worldTransform.transform.translate - radius;

		aabb.max = worldTransform.transform.translate + radius;
	}
	else if (colliderType == ColliderType::typeSphere)
	{
		sphere.center = worldTransform.transform.translate;
		sphere.radius = radius.x;
	}
}

void Collider::Update()
{
	worldTransform.UpdateMatrix();

	worldPosition = { worldTransform.matWorld_.m[3][0],worldTransform.matWorld_.m[3][1],worldTransform.matWorld_.m[3][2] };

	if (colliderType == ColliderType::typeAABB)
	{
		aabb.min = worldPosition - radius;

		aabb.max = worldPosition + radius;
	}
	else if (colliderType == ColliderType::typeSphere)
	{
		sphere.center = worldPosition;
	}
}

void Collider::SetRadius(float radius)
{
	if (colliderType == ColliderType::typeAABB)
	{
		this->radius.x = radius; this->radius.y = radius; this->radius.z = radius;
	}
	else if (colliderType == ColliderType::typeSphere)
	{
		sphere.radius = radius;
	}
}
