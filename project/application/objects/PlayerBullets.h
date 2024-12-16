#pragma once
#include "BaseBullet.h"
#include <list>

class PlayerBullets
{
public:

	void Initialize(Model* model);

	void Update();
	void Draw(ViewProjection viewProjection,DirectionalLight directionalLight,PointLight pointLight,SpotLight spotLight);

	void CreateNewBullet(Object3dCommon* object3dCommon,Vector3 position, Vector3 velocity);
	  
	void Finalize();

	std::list<std::unique_ptr<BaseBullet>>& GetBullets() { return bullets; }

private:

	std::list<std::unique_ptr<BaseBullet>> bullets;

	Model* model = nullptr;

	const float kLifeTime_ = 1000000.0f;
};

