#include "PlayerBullets.h"

void PlayerBullets::Initialize(Model* model)
{
	this->model = new Model();

	this->model = model;
}

void PlayerBullets::Update()
{
	
	auto now = std::chrono::steady_clock::now();

	for (auto it = bullets.begin(); it != bullets.end(); ) {
		float elapsedTime = std::chrono::duration<float>(now - it->get()->GetSpawnTime()).count();

		// ライフタイムを過ぎた弾を削除
		if (elapsedTime >= kLifeTime_ || !it->get()->GetIsAlive()) {

			it = bullets.erase(it); // eraseは次のイテレータを返す

		}
		else {
			it->get()->Update(); // 更新処理を行う
			++it;         // 次のイテレータへ
		}
	}
}

void PlayerBullets::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{

	for (auto it = bullets.begin(); it != bullets.end(); ) {
		it->get()->Draw(viewProjection, directionalLight, pointLight, spotLight);
		++it;
	}
}

void PlayerBullets::CreateNewBullet(Object3dCommon* object3dCommon, Vector3 position, Vector3 velocity)
{

	std::unique_ptr<BaseBullet> newBullet = std::make_unique<BaseBullet>();
	newBullet->Initialize(object3dCommon, position);
	newBullet->SetModel(model);
	newBullet->SetVelocity(velocity);
	bullets.push_back(std::move(newBullet));
}

void PlayerBullets::Finalize()
{
	bullets.clear();
}


