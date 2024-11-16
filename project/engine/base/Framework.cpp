#include "Framework.h"

void Framework::Initialize()
{

	/*-----------------------------------
	* WinAppの生成と初期化
	-----------------------------------*/

	winApp = std::make_unique<WinApp>();
	winApp->Initialize();

	/*-----------------------------------
	* DirectXCommonの生成と初期化
	-----------------------------------*/

	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	/*-----------------------------------
	* Inputの生成と初期化
	-----------------------------------*/


	input = std::make_unique<Input>();
	input->Initialize(winApp.get());

	/*-----------------------------------
	* Audioの生成と初期化
	-----------------------------------*/


	audio = std::make_unique<Audio>();
	audio->Initialize();

	/*-----------------------------------
	* SrvManagerの生成と初期化
	-----------------------------------*/


	srvManager = std::make_unique<SrvManager>();
	srvManager->Initialize(dxCommon.get());

	/*-----------------------------------
	* TextureManagerの初期化
	-----------------------------------*/

	TextureManager::GetInstance()->Initialize(dxCommon.get(), srvManager.get());

	/*-----------------------------------
	* SpriteCommonの生成と初期化
	*
	*  スプライト共通部
	-----------------------------------*/


	spriteCommon = std::make_unique<SpriteCommon>();
	spriteCommon->Initialize(dxCommon.get());

	/*-----------------------------------
	* Object3dCommonの生成と初期化
	*
	*  3Dオブジェクト共通部
	-----------------------------------*/


	object3dCommon = std::make_unique<Object3dCommon>();
	object3dCommon->Initialize(dxCommon.get());

	/*-----------------------------------
	* ModelCommonの生成と初期化
	*
	*  モデル共通部
	-----------------------------------*/


	modelCommon = std::make_unique<ModelCommon>();
	modelCommon->Initialize(dxCommon.get());

	/*-----------------------------------
	* ModelManagerの初期化
	-----------------------------------*/

	ModelManager::GetInstance()->Initialize(dxCommon.get());

	/*-----------------------------------
	* Cameraの初期化
	-----------------------------------*/


	camera = std::make_unique<Camera>();

	/*-----------------------------------
	* ParticleManagerの初期化
	-----------------------------------*/

	ParticleManager::GetInstance()->Initialize(dxCommon.get(), srvManager.get(), camera.get());
	ParticleManager::GetInstance()->CreateParticleGroup("particle", "Resources/circle.png");

	/*-----------------------------------
	* ParticleEmitterの生成と初期化
	-----------------------------------*/


	particleEmitter = std::make_unique<ParticleEmitter>();
	particleEmitter->Initialize("particle");
	particleEmitter->Emit();

}

void Framework::Finalize()
{

	ParticleManager::GetInstance()->Finalize();

	ModelManager::GetInstance()->Finalize();

	TextureManager::GetInstance()->Finalize();

	audio->SoundUnload();

	winApp->Finalize();

}

void Framework::Update()
{

	if (winApp->ProcessMessage()) {
		endRequest_ = true;
	}

}

void Framework::Draw()
{
}

void Framework::Run()
{

	// ゲームの初期化
	Initialize();

	while (true)
	{

		// 毎フレーム更新
		Update();

		if (IsEndRequest())
		{
			break;
		}

		Draw();
	}

	// ゲームの終了
	Finalize();
}
