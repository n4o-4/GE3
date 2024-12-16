#include "Player.h"

void Player::Initialize(Model* bulletModel)
{
	bullets = std::make_unique<PlayerBullets>();
	bullets->Initialize(bulletModel);

	canFire = true;

	worldTransform_.Initialize();

	reticleTransform_.Initialize();

}

void Player::Update(ViewProjection viewPro)
{
	worldTransform_.UpdateMatrix();

	// レティクルの計算
	
		//
		const float kDistancePlayer3DReticle = 50.0f;

		Vector3 offset = { 0.0f,0.0f,1.0f };

		offset = offset * kDistancePlayer3DReticle;

		reticleTransform_.transform.translate = worldTransform_.transform.translate + offset;

		reticleTransform_.UpdateMatrix();

		// スクリーン座標に変換
		positionReticle = { reticleTransform_.matWorld_.m[3][0],reticleTransform_.matWorld_.m[3][1], reticleTransform_.matWorld_.m[3][2] };

		Matrix4x4 matViewport = MakeViewportMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 1.0f);

		Matrix4x4 matViewProjectionViewPort = Multiply(Multiply(viewPro.matView_,viewPro.matProjection_),matViewport);

		positionReticle = MakeTransform(positionReticle, matViewProjectionViewPort);

	reticleTransform_.UpdateMatrix();

	bullets->Update();

	
}

void Player::Draw(ViewProjection view, DirectionalLight dirLight, PointLight pointLight, SpotLight spotLight)
{
	bullets->Draw(view, dirLight, pointLight, spotLight);
}

void Player::MoveRight()
{
	
	worldTransform_.transform.translate.x += 0.1f;
	
}

void Player::MoveLeft()
{
	
	worldTransform_.transform.translate.x -= 0.1f;
	
}

void Player::CheckFireInput()
{
	auto now = std::chrono::steady_clock::now();

	float elapsedTime = std::chrono::duration<float>(now - lastFireTime_).count();

	if (elapsedTime >= kIntervalTime)
	{
		bullets->CreateNewBullet(Object3dCommon::GetInstance(), worldTransform_.transform.translate);

		lastFireTime_ = now;
	}
}
