#pragma once
#include "Structs.h"
#include "EnemyBullets.h"
#include "Collider.h"

class Enemy : public Collider
{
public:

	void Initialize(Model* model,Model* bulletModel);

	void Update(Vector3 target);

	void Draw(ViewProjection view, DirectionalLight dirLight, PointLight pointLight, SpotLight spotLight);

	Vector3& GetTranslate() { return worldTransform.transform.translate; }
	void SetTranslate(Vector3 translate) { this->worldTransform.transform.translate = translate; }

	void OnCollision() override;
	
	bool CheckIsAlive() { return isAlive_; }

	WorldTransform GetWorldTransform() { return worldTransform; }

	std::list<std::unique_ptr<BaseBullet>>& GetBullet() { return bullets->GetBullets(); }

private: // メンバ関数

	void FireBullet(Vector3 target);

private:

	std::unique_ptr<EnemyBullets> bullets = nullptr;

	const  float kIntervalTime = 5.0f;

	Model* model = nullptr;

	bool isAlive_ = true;

	std::chrono::steady_clock::time_point lastFireTime_;
};