#pragma once
#include "BaseBullet.h"
#include <list>

class EnemyBullets
{
public:
	// 初期化
	void Initialize(Model* model);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	// 弾の生成
	void CreateNewBullet(Object3dCommon* object3dCommon, Vector3 position,Vector3 velocity);

	std::list<std::unique_ptr<BaseBullet>>& GetBullets() { return bullets; }

private:

	std::list<std::unique_ptr<BaseBullet>> bullets;

	Model* model = nullptr;

	const float kLifeTime_ = 5.0f;
};

