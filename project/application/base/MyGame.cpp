#include "MyGame.h"


void MyGame::Initialize()
{

#pragma region 基盤システムの初期化

	
	Framework::Initialize();

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


	sprite = std::make_unique<Sprite>();

	sprite->Initialize(spriteCommon.get(),"Resources/uvChecker.png");

	sprite->SetTexSize({ 512.0f,512.0f });
}

void MyGame::Finalize()
{

	Framework::Finalize();

}

void MyGame::Update()
{

	Framework::Update();

	sprite->SetPosition({ 0.0f,0.0f });

	sprite->Update();


#ifdef _DEBUG

	imGuiManager->Begin();

	imGuiManager->End();

#endif

	Vector3 cameraTranslate = camera->GetTranslate();

	camera->SetTranslate(cameraTranslate);
}

void MyGame::Draw()
{

	Framework::Draw();

	dxCommon->PreDraw();

	srvManager->PreDraw();

	object3dCommon->SetView();

	camera->Update();

	spriteCommon->SetView();

	sprite->Draw();

#ifdef _DEBUG

	imGuiManager->Draw(dxCommon.get());

#endif

	dxCommon->PostDraw();

}
