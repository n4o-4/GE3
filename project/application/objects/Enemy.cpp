#include "Enemy.h"

void Enemy::Initialize(Model* bulletModel)
{
	Collider::Initialize(ColliderType::typeAABB);
	Collider::SetRadius(1.0f);

	bullets = std::make_unique<EnemyBullets>();
	bullets->Initialize(bulletModel);

	canFire = true;

	//worldTransform.Initialize();

	model = new Model();
	model =  bulletModel;

	isAlive_ = true;
}

void Enemy::Update()
{
	Collider::Update();

	bullets->Update();

	worldTransform.UpdateMatrix();
}

void Enemy::Draw(ViewProjection view, DirectionalLight dirLight, PointLight pointLight, SpotLight spotLight)
{
	bullets->Draw(view, dirLight, pointLight, spotLight);

	model->Draw(worldTransform);
}

void Enemy::OnCollision()
{
	isAlive_ = false;
}
