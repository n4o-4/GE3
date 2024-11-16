#pragma once
#include <memory>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Object3dCommon.h"
#include "ModelCommon.h"
#include "ModelManager.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "ImGuiManager.h"

// ゲーム全体
class MyGmae
{
public:
private:
public: //　メンバ関数
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム
	void Update();

	// 描画
	void Draw();
};