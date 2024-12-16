#pragma once
#include <chrono>
#include "Object3dCommon.h"
#include "Object3d.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Collider.h"

class BaseBullet : public Collider
{
public:

	void Initialize(Object3dCommon* object3dCommon,Vector3 initialPosition);
	void SetModel(Model* model);
	void Update();
	void Draw(ViewProjection viewProjection,DirectionalLight directionalLight,PointLight pointLight,SpotLight spotLight);
	std::chrono::steady_clock::time_point GetSpawnTime() { return spawnTime_; }

	void OnCollision() override;

protected:

	Vector3 velocity_;
	//WorldTransform* worldTransform_ = nullptr;
	Object3d* object3d_ = nullptr;
	std::chrono::steady_clock::time_point spawnTime_;
	bool isAlive_ = true;
};

