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
		float elapsedTime = std::chrono::duration<float>(now - it->GetSpawnTime()).count();

		// ライフタイムを過ぎた弾を削除
		if (elapsedTime >= kLifeTime_) {
			it = bullets.erase(it); // eraseは次のイテレータを返す
		}
		else {
			it->Update(); // 更新処理を行う
			++it;         // 次のイテレータへ
		}
	}
}

void PlayerBullets::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	for (auto bullet : bullets)
	{
		bullet.Draw(viewProjection,directionalLight,pointLight,spotLight);
	}
}

void PlayerBullets::CreateNewBullet(Object3dCommon* object3dCommon,Vector3 position)
{

	BaseBullet* newBullet = new BaseBullet();
	newBullet->Initialize(object3dCommon, position);
	newBullet->SetModel(model);

	bullets.push_back(*newBullet);
}

void PlayerBullets::Finalize()
{
	for (auto it = bullets.begin(); it != bullets.end(); ) 
	{
			it = bullets.erase(it); // eraseは次のイテレータを返す
	}
}


