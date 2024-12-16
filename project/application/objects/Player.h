#pragma once
#include "Structs.h"
#include "PlayerBullets.h"

class Player
{
public:

	void Initialize(Model* bulletModel);

	void Update();
	void Draw(ViewProjection view,DirectionalLight dirLight,PointLight pointLight,SpotLight spotLight);

	void MoveRight();
	void MoveLeft();
	
	void CheckFireInput();

	Vector3 GetTranslate() { return translate; }

private:
	Vector3 translate = { 0.0f,0.0f,0.0f };

	PlayerBullets* bullets = nullptr;

	bool canFire = true;

	const  float kIntervalTime = 1.0f;

	std::chrono::steady_clock::time_point lastFireTime_;
};