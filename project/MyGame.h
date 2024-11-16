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
#include <dxgidebug.h>

// ゲーム全体
class MyGame
{
public:
private:

	std::unique_ptr<WinApp> winApp = nullptr;

	std::unique_ptr<DirectXCommon> dxCommon = nullptr;

	std::unique_ptr<Input> input = nullptr;

	std::unique_ptr<Audio> audio = nullptr;

	std::unique_ptr<SrvManager> srvManager = nullptr;

	std::unique_ptr<SpriteCommon> spriteCommon = nullptr;

	std::unique_ptr<Object3dCommon> object3dCommon = nullptr;

	std::unique_ptr<ModelCommon> modelCommon = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

	std::unique_ptr<ParticleEmitter> particleEmitter = nullptr;

	std::unique_ptr<ImGuiManager> imGuiManager = nullptr;

	// ゲーム終了フラグ
	bool endRequest_ = false;

public: //　メンバ関数
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム
	void Update();

	// 描画
	void Draw();

	// 終了フラグのチェック
	bool IsEndRequest() { return endRequest_; }

private:

	
};