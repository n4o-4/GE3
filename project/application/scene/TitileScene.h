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
#include "BaseScene.h"

class TitleScene : public BaseScene
{
public:

	std::unique_ptr<Sprite> sprite = nullptr;

	std::unique_ptr<Object3d> object3d = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

private:
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