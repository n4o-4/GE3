#include "Enemy.h"

void Enemy::Initialize(Model* model,Model* bulletModel)
{
	Collider::Initialize(ColliderType::typeAABB);
	Collider::SetRadius(1.0f);

	bullets = std::make_unique<EnemyBullets>();
	bullets->Initialize(bulletModel);

	//worldTransform.Initialize();

	this->model = new Model();
	this->model =  model;

	isAlive_ = true;
}

void Enemy::Update(Vector3 target)
{
	Collider::Update();

	FireBullet(target);

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

void Enemy::FireBullet(Vector3 target)
{
	// 現在の時刻を取得
	auto now = std::chrono::steady_clock::now();

	// 前回弾を撃ってからの経過時間を計測
	float elapsedTime = std::chrono::duration<float>(now - lastFireTime_).count();

	// 経過時間がインターバルより短い場合 早期に　return 
	if (elapsedTime <= kIntervalTime)
	{
		return;
	}

	Vector3 velocity = Normalize({ target - worldTransform.transform.translate });

	// 新しい弾を作る
	bullets->CreateNewBullet(Object3dCommon::GetInstance(), worldTransform.transform.translate, velocity);

	// 最後に撃った時間を変数に記録
	lastFireTime_ = now;
}
