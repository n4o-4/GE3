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
#include "Audio.h"

class TitleScene : public BaseScene
{
private:

	std::unique_ptr<Camera> camera = nullptr;

	// スカイドーム
	std::unique_ptr<Object3d> skydome_ = nullptr;
	std::unique_ptr<WorldTransform> skydomeTransform_ = nullptr;

	// タイトル
	std::unique_ptr<Object3d> title = nullptr;
	std::unique_ptr<WorldTransform> titleTransform = nullptr;

	// エンターキー
	std::unique_ptr<Object3d> enter_ = nullptr;
	std::unique_ptr<WorldTransform> enterTrans_ = nullptr;

	// ライト
	std::unique_ptr<DirectionalLight> directionalLight = nullptr;
	std::unique_ptr<PointLight> pointLight = nullptr;
	std::unique_ptr<SpotLight> spotLight = nullptr;


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