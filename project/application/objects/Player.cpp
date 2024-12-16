#include "Player.h"

void Player::Initialize(Model* bulletModel)
{
	Collider::Initialize(ColliderType::typeAABB);
	Collider::SetRadius(1.0f);

	isAlive_ = true;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());
	object3d_->SetModel("AL4_Player.obj");

	object3d_->SetLocalMatrix(MakeIdentity4x4());

	bullets = std::make_unique<PlayerBullets>();
	bullets->Initialize(bulletModel);

	canFire = true;

	reticleTransform_.Initialize();

	HP = kMaxHP;
}

void Player::Update(ViewProjection viewPro)
{

	if (!isAlive_)
	{
		return;
	}

	if (HP <= 0)
	{
		isAlive_ = false;
	}

	object3d_->Update();

	if (Input::GetInstance()->PushKey(DIK_W))
	{
		worldTransform.transform.translate.y += 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_S))
	{
		worldTransform.transform.translate.y -= 0.1f;
	}

	Collider::Update();

	// レティクルの計算
	
		//
	const float kDistancePlayer3DReticle = 50.0f;

	Vector3 offset = { 0.0f,0.0f,1.0f };

	offset = offset * kDistancePlayer3DReticle;

	reticleTransform_.transform.translate = worldTransform.transform.translate + offset;

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
	object3d_->Draw(worldTransform, view, dirLight, pointLight, spotLight);

	bullets->Draw(view, dirLight, pointLight, spotLight);
}

void Player::MoveRight()
{
	
	worldTransform.transform.translate.x += 0.1f;
	
}

void Player::MoveLeft()
{
	
	worldTransform.transform.translate.x -= 0.1f;
	
}

void Player::CheckFireInput()
{
	if (!isAlive_)
	{
		return;
	}

	// 現在の時刻を取得
	auto now = std::chrono::steady_clock::now();

	// 前回弾を撃ってからの経過時間を計測
	float elapsedTime = std::chrono::duration<float>(now - lastFireTime_).count();

	// 経過時間がインターバルより短い場合 早期に　return 
	if (elapsedTime <= kIntervalTime)
	{
		return;
	}

	Vector3 velocity = { 0.0f,0.0f,1.0f };

	// もしロックオンが有効だった場合velocityを上書き 無効だった場合そのまま
	if (isLockOn_)
	{
		// 敵に向かって撃つように差分ベクトルを計算
		velocity = Normalize({ targetPosition - worldTransform.transform.translate});
	}

	// 新しい弾を作る
	bullets->CreateNewBullet(Object3dCommon::GetInstance(), worldTransform.transform.translate,velocity);

	// 最後に撃った時間を変数に記録
	lastFireTime_ = now;
}

void Player::Finalize()
{
	bullets->Finalize();
	bullets.release();
}

void Player::OnCollision()
{
	if (isAlive_)
	{
		--HP;
	}
}
