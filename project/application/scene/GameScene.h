#pragma once

#include "BaseScene.h"

#include "audio.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"

#include "WorldTransform.h"

#include "AnimationManager.h"
#include "InputHandler.h"
#include "Command.h"
#include "Player.h"
#include "Enemy.h"

class GameScene : public BaseScene
{
public:

	std::unique_ptr<Sprite> sprite = nullptr;

	std::unique_ptr<Object3d> object3d = nullptr;

	std::unique_ptr<WorldTransform> objectTransform = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

	std::unique_ptr<DirectionalLight> directionalLight = nullptr;

	std::unique_ptr<PointLight> pointLight = nullptr;

	std::unique_ptr<SpotLight> spotLight = nullptr;

	//std::unique_ptr<AnimationManager> animationManager = nullptr;

	InputHandler* inputHandler_ = nullptr;
	ICommand* iCommand_ = nullptr;


	std::unique_ptr<Player> player_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemys_;

	//bool isLockOn_ = false;

	Vector2 drawReticlrPosition;

	std::unique_ptr<Sprite> reticle = nullptr;
	std::unique_ptr<Sprite> lockOn_Reticle = nullptr;

	std::unique_ptr<Object3d> skydome_ = nullptr;
	std::unique_ptr<WorldTransform> skydomeTransform_ = nullptr;

	std::unique_ptr<Object3d> space = nullptr;
	std::unique_ptr<WorldTransform> spaceTrans_ = nullptr;

	bool isPushSpace = false;

	bool isClear_ = false;

	int Count = 0;

private:

	bool CheckCollision(Collider colliderA, Collider colliderB);


	void CheckAllCollision();

	void CalculationReticle();


public: // メンバ関数

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;
};