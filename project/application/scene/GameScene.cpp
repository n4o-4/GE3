#include "GameScene.h"

void GameScene::Initialize()
{

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");

	Audio::GetInstance()->SoundPlayWave("Resources/Alarm01.wav");

	sprite = std::make_unique<Sprite>();

	sprite->Initialize(SpriteCommon::GetInstance(), "Resources/uvChecker.png");

	sprite->SetTexSize({ 512.0f,512.0f });

	ModelManager::GetInstance()->LoadModel("axis.obj");

	camera = std::make_unique<Camera>();

	object3d = std::make_unique<Object3d>();

	object3d->Initialize(Object3dCommon::GetInstance());

	object3d->SetModel("axis.obj");

	object3d->SetCamera(camera.get());
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{

	camera->SetTranslate({ 0.0f,0.0f,-15.0f });

	camera->Update();

	sprite->Update();

	Vector3 rotato = object3d->GetRotation();

	rotato.y += 0.02f;

	object3d->SetRotation(rotato);
	
	Vector3 translate = object3d->GetTranslate();

	translate.z += 0.1f;

	object3d->SetTranslate(translate);

	object3d->Update();

	if (Input::GetInstance()->Triggerkey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameScene::Draw()
{

	Object3dCommon::GetInstance()->SetView();

	object3d->Draw();

	SpriteCommon::GetInstance()->SetView();

	sprite->Draw();

}
