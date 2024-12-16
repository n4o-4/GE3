#pragma once
#include "Structs.h"
#include "EnemyBullets.h"
#include "Collider.h"

class Enemy : public Collider
{
public:

	void Initialize(Model* bulletModel);

	void Update();
	void Draw(ViewProjection view, DirectionalLight dirLight, PointLight pointLight, SpotLight spotLight);

	Vector3& GetTranslate() { return worldTransform.transform.translate; }
	void SetTranslate(Vector3 translate) { this->worldTransform.transform.translate = translate; }

	void OnCollision() override;
	
	bool CheckIsAlive() { return isAlive_; }

	WorldTransform GetWorldTransform() { return worldTransform; }

private:
	//Vector3 translate = { 0.0f,0.0f,0.0f };

	std::unique_ptr<EnemyBullets> bullets = nullptr;

	bool canFire = true;

	const  float kIntervalTime = 0.3f;

	Model* model = nullptr;

	//WorldTransform worldTransform;

	bool isAlive_ = true;
};