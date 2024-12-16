#pragma once
#include "Structs.h"
#include "PlayerBullets.h"

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

public:

	std::list<BaseBullet> GetBullet() { return bullets->GetBullets(); }

};