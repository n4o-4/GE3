#include "TitleScene.h"

void TitleScene::Initialize()
{
	BaseScene::Initialize();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	ModelManager::GetInstance()->LoadModel("AL4_Skydome.obj");
	ModelManager::GetInstance()->LoadModel("AL4_Title.obj");
	ModelManager::GetInstance()->LoadModel("Enter.obj");

	// スカイドームのモデル
	skydome_ = std::make_unique<Object3d>();
	skydome_->Initialize(Object3dCommon::GetInstance());
	skydome_->SetModel(ModelManager::GetInstance()->FindModel("AL4_Skydome.obj"));

	// スカイドームの座標
	skydomeTransform_ = std::make_unique<WorldTransform>();
	skydomeTransform_->Initialize();

	// タイトルのモデル
	title = std::make_unique<Object3d>();
	title->Initialize(Object3dCommon::GetInstance());
	title->SetModel(ModelManager::GetInstance()->FindModel("AL4_Title.obj"));
	
	//ModelManager::GetInstance()->FindModel("AL4_Title.obj")->SetEnableLighting(false);

	// タイトルの座標
	titleTransform = std::make_unique<WorldTransform>();
	titleTransform->Initialize();

	titleTransform->transform.rotate = {1.74f,3.14f,0.0f};
	enter_ = std::make_unique<Object3d>();
	enter_->Initialize(Object3dCommon::GetInstance());
	enter_->SetModel(ModelManager::GetInstance()->FindModel("Enter.obj"));
	// 

	enterTrans_ = std::make_unique<WorldTransform>();
	enterTrans_->Initialize();
	enterTrans_->transform.rotate = { 1.74f,3.14f,0.0f };
	enterTrans_->transform.translate = { 0.0f,-1.0f,0.0f };

	// ライトの生成と初期化
	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();

	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();

	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	camera->Update();

	if (Input::GetInstance()->Triggerkey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("GAME");

		return;
	}

	directionalLight->Update();
	pointLight->Update();
	spotLight->Update();

	skydomeTransform_->UpdateMatrix();

	titleTransform->UpdateMatrix();

	enterTrans_->UpdateMatrix();
}

void TitleScene::Draw()
{
	Object3dCommon::GetInstance()->SetView();

	skydome_->Draw(*skydomeTransform_.get(), camera->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	title->Draw(*titleTransform.get(), camera->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	enter_->Draw(*enterTrans_.get(), camera->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	SpriteCommon::GetInstance()->SetView();
}
