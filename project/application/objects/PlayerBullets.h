#pragma once
#include "BaseBullet.h"
#include <list>

class PlayerBullets
{
public:

	void Initialize(Model* model);

	void Update();
	void Draw(ViewProjection viewProjection,DirectionalLight directionalLight,PointLight pointLight,SpotLight spotLight);

	void CreateNewBullet(Object3dCommon* object3dCommon,Vector3 position);
	  
	void Finalize();

	std::list<BaseBullet> GetBullets() { return bullets; }

private:

	std::list<BaseBullet> bullets;

	Model* model = nullptr;

	const float kLifeTime_ = 10.0f;
};

