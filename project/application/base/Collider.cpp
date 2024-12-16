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
	if (colliderType == ColliderType::typeAABB)
	{
		aabb.min = worldTransform.transform.translate - radius;

		aabb.max = worldTransform.transform.translate + radius;
	}
	else if (colliderType == ColliderType::typeSphere)
	{
		sphere.center = worldTransform.transform.translate;
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
