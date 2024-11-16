#include "MyGame.h"


void MyGame::Initialize()
{

#pragma region 基盤システムの初期化

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

#ifdef _DEBUG

	
	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp.get(), dxCommon.get());

#endif

#pragma endregion 基盤システムの初期化

	audio->SoundLoadWave("Resources/Alarm01.wav");
	audio->SoundPlayWave();

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

}

void MyGame::Finalize()
{

	ParticleManager::GetInstance()->Finalize();

	ModelManager::GetInstance()->Finalize();

	TextureManager::GetInstance()->Finalize();

	audio->SoundUnload();

	winApp->Finalize();

}

void MyGame::Update()
{



	if (winApp->ProcessMessage()) {
		endRequest_ = true;
	}

#ifdef _DEBUG

	imGuiManager->Begin();

	imGuiManager->End();

#endif

	Vector3 cameraTranslate = camera->GetTranslate();

	camera->SetTranslate(cameraTranslate);

	dxCommon->PreDraw();

	srvManager->PreDraw();

	object3dCommon->SetView();

	camera->Update();

	

	spriteCommon->SetView();



#ifdef _DEBUG

	imGuiManager->Draw(dxCommon.get());

#endif

	dxCommon->PostDraw();

}

void MyGame::Draw()
{
}
