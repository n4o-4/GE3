#include "MyGame.h"

#include "SceneFactory.h"


void MyGame::Initialize()
{

#pragma region 基盤システムの初期化

	
	Framework::Initialize();

#ifdef _DEBUG
	
	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp.get(), dxCommon.get());

#endif

	sceneFactory_ = std::make_unique<SceneFactory>();

	SceneManager::GetInstance()->SetSceneFactory(*sceneFactory_);

	SceneManager::GetInstance()->ChangeScene("TITLE");

#pragma endregion 基盤システムの初期化

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

}

void MyGame::Finalize()
{

	Framework::Finalize();

}

void MyGame::Update()
{

	Framework::Update();

#ifdef _DEBUG

	imGuiManager->Begin();

	imGuiManager->End();

#endif

	Vector3 cameraTranslate = camera->GetTranslate();

	camera->SetTranslate(cameraTranslate);

	camera->Update();
}

void MyGame::Draw()
{
	dxCommon->PreDraw();

	srvManager->PreDraw();

	Framework::Draw();
	

	

#ifdef _DEBUG

	imGuiManager->Draw(dxCommon.get());

#endif

	dxCommon->PostDraw();

}
