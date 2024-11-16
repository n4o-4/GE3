#pragma once

#include <memory>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3dCommon.h"
#include "ModelCommon.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "ImGuiManager.h"


class Framework
{
public:

protected:

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


	bool endRequest_ = false;

public:

	virtual~Framework() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 毎フレーム更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// 終了チェック
	virtual bool IsEndRequest() { return endRequest_; }

	// 実行
	void Run();

};
