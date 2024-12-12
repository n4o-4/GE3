#include "Framework.h"

#include "LeakChecker.h"

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

	DirectXCommon::GetInstance()->Initialize(winApp.get());

	/*-----------------------------------
	* Audioの初期化
	-----------------------------------*/

	

	/*-----------------------------------
	* Inputの生成と初期化
	-----------------------------------*/

	Input::GetInstance()->Initialize(winApp.get());

	/*-----------------------------------
	* SrvManagerの生成と初期化
	-----------------------------------*/


	srvManager = std::make_unique<SrvManager>();
	srvManager->Initialize(DirectXCommon::GetInstance());

	/*-----------------------------------
	* TextureManagerの初期化
	-----------------------------------*/

	TextureManager::GetInstance()->Initialize(DirectXCommon::GetInstance(), srvManager.get());

	/*-----------------------------------
	* SpriteCommonの生成と初期化
	*
	*  スプライト共通部
	-----------------------------------*/

	SpriteCommon::GetInstance()->Initialize(DirectXCommon::GetInstance());

	/*-----------------------------------
	* Object3dCommonの生成と初期化
	*
	*  3Dオブジェクト共通部
	-----------------------------------*/


	Object3dCommon::GetInstance()->Initialize(DirectXCommon::GetInstance());

	/*-----------------------------------
	* ModelCommonの生成と初期化
	*
	*  モデル共通部
	-----------------------------------*/

	modelCommon = std::make_unique<ModelCommon>();
	modelCommon->Initialize(DirectXCommon::GetInstance());


	/*-----------------------------------
	* ModelManagerの初期化
	-----------------------------------*/

	ModelManager::GetInstance()->Initialize(DirectXCommon::GetInstance());

	/*-----------------------------------
	* Cameraの初期化
	-----------------------------------*/

	Camera::GetInstance()->Initialize();

	/*-----------------------------------
	* ParticleManagerの初期化
	-----------------------------------*/

	ParticleManager::GetInstance()->Initialize(DirectXCommon::GetInstance(), srvManager.get(), Camera::GetInstance());
	//ParticleManager::GetInstance()->CreateParticleGroup("particle", "Resources/circle.png");

	/*-----------------------------------
	* ParticleEmitterの生成と初期化
	-----------------------------------*/


	/*particleEmitter = std::make_unique<ParticleEmitter>();
	particleEmitter->Initialize("particle");
	particleEmitter->Emit();*/

	SceneManager::GetInstance()->Initialize();
}

void Framework::Finalize()
{

	SceneManager::GetInstance()->Finalize(); ///

	ParticleManager::GetInstance()->Finalize(); ///

	Camera::GetInstance()->Finalize();

	ModelManager::GetInstance()->Finalize(); ///

	Object3dCommon::GetInstance()->Finalize(); ///

	SpriteCommon::GetInstance()->Finalize(); ///

	TextureManager::GetInstance()->Finalize(); ///

	Input::GetInstance()->Finalize(); //

	//Audio::GetInstance()->Finalize(); //

	DirectXCommon::GetInstance()->Finalize();

	winApp->Finalize();

}

void Framework::Update()
{

	if (winApp->ProcessMessage()) {
		endRequest_ = true;
	}

	Input::GetInstance()->Update();

	SceneManager::GetInstance()->Update();

}

void Framework::Draw()
{

	SceneManager::GetInstance()->Draw();

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
