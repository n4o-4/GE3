#pragma once

#include "audio.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"

class GameScene
{
public:

	std::unique_ptr<Sprite> sprite = nullptr;

	std::unique_ptr<Object3d> object3d = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

private:
public: // メンバ関数

	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();
};