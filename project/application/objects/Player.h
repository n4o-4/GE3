#pragma once
#include "Structs.h"
#include "PlayerBullets.h"
#include "Input.h"

class Player
{
public:

	void Initialize(Model* bulletModel);

	void Update(ViewProjection viewPro);
	void Draw(ViewProjection view,DirectionalLight dirLight,PointLight pointLight,SpotLight spotLight);

	void MoveRight();
	void MoveLeft();
	
	void CheckFireInput();

	WorldTransform GetWorldTransform() { return worldTransform_; }

	Vector3 Get2DReticlePosition() { return positionReticle; }

	bool GetIsLockOn() { return isLockOn_; }

	void SetIsLockOn(bool flag) { isLockOn_ = flag; }

	void Finalize();

	void SetTargetPosition(Vector3 position) { targetPosition = position; }

private:

	Vector3 translate = { 0.0f,0.0f,0.0f };

	Vector3 positionReticle;

	std::unique_ptr<PlayerBullets> bullets = nullptr;

	bool canFire = true;

	const  float kIntervalTime = 0.3f;

	std::chrono::steady_clock::time_point lastFireTime_;

	WorldTransform worldTransform_;

	WorldTransform reticleTransform_;

	ViewProjection* viewPro_ = nullptr;

	bool isLockOn_ = false;

	Vector3 targetPosition;

public:

	std::list<std::unique_ptr<BaseBullet>>& GetBullet() { return bullets->GetBullets(); }

};