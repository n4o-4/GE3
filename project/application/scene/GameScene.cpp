#include "GameScene.h"

#include "imgui.h"

void GameScene::Initialize()
{

	/*TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");*/

	//Audio::GetInstance()->SoundPlayWave("Resources/Alarm01.wav");

	sprite = std::make_unique<Sprite>();

	sprite->Initialize(SpriteCommon::GetInstance(), "Resources/uvChecker.png");

	sprite->SetTexSize({ 512.0f,512.0f });

	ModelManager::GetInstance()->LoadModel("plane.gltf");

	//camera = std::make_unique<Camera>();

	object3d = std::make_unique<Object3d>();

	object3d->Initialize(Object3dCommon::GetInstance());

	object3d->SetModel("plane.gltf");

	object3d->SetCamera(camera.get());

	objectTransform = std::make_unique<WorldTransform>();
	objectTransform->Initialize();

	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();

	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();

	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();

	//camera->SetTranslate({ 0.0f,2.0f,-10.0f });

	//camera->SetRotate({ 0.2f,0.0f,0.0f });
}

void GameScene::Finalize()
{
	
}

void GameScene::Update()
{
	//camera->Update();

	sprite->Update();

	//objectTransform->transform.rotate.y += 0.01f;

#ifdef _DEBUG

	/*Vector3 translate = camera->GetTranslate();

	ImGui::DragFloat3("cameraTranslate", &translate.x, 0.01f);

	camera->SetTranslate(translate);

	Vector3 rotate = camera->GetRotate();

	ImGui::DragFloat3("cameraRotate", &rotate.x, 0.01f);

	camera->SetRotate(rotate);*/
	
	ImGui::DragFloat3("object.translate", &objectTransform->transform.translate.x,0.01f);

	ImGui::DragFloat3("object.rotate", &objectTransform->transform.rotate.x, 0.01f);

	ImGui::DragFloat3("Object.scale", &objectTransform->transform.scale.x, 0.01f);

	ImGui::DragFloat3("pointLight.position", &pointLight->position_.x, 0.01f);

	ImGui::DragFloat("pointLight.decay", &pointLight->decay_, 0.01f);

	ImGui::DragFloat("pointLight.radius", &pointLight->radius_, 0.01f);

	ImGui::DragFloat("pointLight.intensity", &pointLight->intensity_, 0.01f);

#endif

	object3d->Update();

	if (Input::GetInstance()->Triggerkey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	pointLight->Update();

	spotLight->Update();
}

void GameScene::Draw()
{

	SpriteCommon::GetInstance()->SetView();

	//sprite->Draw();

	Object3dCommon::GetInstance()->SetView();

	objectTransform->UpdateMatrix();

	object3d->Draw(*objectTransform.get(),Camera::GetInstance()->GetViewProjection(),*directionalLight.get(), *pointLight.get(), *spotLight.get());

}
