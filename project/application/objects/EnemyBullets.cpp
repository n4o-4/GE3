#include "EnemyBullets.h"

void EnemyBullets::Initialize(Model* model)
{
	this->model = new Model();

	this->model = model;
}

void EnemyBullets::Update()
{
	auto now = std::chrono::steady_clock::now();

	for (auto it = bullets.begin(); it != bullets.end(); ) {
		float elapsedTime = std::chrono::duration<float>(now - it->get()->GetSpawnTime()).count();

		// ライフタイムを過ぎた弾を削除
		if (elapsedTime >= kLifeTime_) {
			it = bullets.erase(it); // eraseは次のイテレータを返す
		}
		else {
			it->get()->Update(); // 更新処理を行う
			++it;         // 次のイテレータへ
		}
	}
}

void EnemyBullets::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	/*for (auto bullet : bullets)
	{
		bullet.Draw(viewProjection, directionalLight, pointLight, spotLight);
	}*/

	for (auto it = bullets.begin(); it != bullets.end(); ) {
		it->get()->Draw(viewProjection, directionalLight, pointLight, spotLight);
	}
}

void EnemyBullets::CreateNewBullet(Object3dCommon* object3dCommon, Vector3 position)
{
	std::unique_ptr<BaseBullet> newBullet = std::make_unique<BaseBullet>();
	newBullet->Initialize(object3dCommon, position);
	newBullet->SetModel(model);

	bullets.push_back(std::move(newBullet));
}
