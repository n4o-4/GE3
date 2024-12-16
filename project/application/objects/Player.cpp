#include "Player.h"

void Player::Initialize(Model* bulletModel)
{
	bullets = new PlayerBullets();
	bullets->Initialize(bulletModel);

	canFire = true;
}

void Player::Update()
{
	bullets->Update();
}

void Player::Draw(ViewProjection view, DirectionalLight dirLight, PointLight pointLight, SpotLight spotLight)
{
	bullets->Draw(view, dirLight, pointLight, spotLight);
}

void Player::MoveRight()
{
	
	translate.x += 0.1f;
	
}

void Player::MoveLeft()
{
	
	translate.x -= 0.1f;
	
}

void Player::CheckFireInput()
{
	auto now = std::chrono::steady_clock::now();

	float elapsedTime = std::chrono::duration<float>(now - lastFireTime_).count();

	if (elapsedTime >= kIntervalTime)
	{
		bullets->CreateNewBullet(Object3dCommon::GetInstance(), translate);

		lastFireTime_ = now;
	}
}
