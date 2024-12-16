#include "GameScene.h"

#include "imgui.h"

void ShowMatrix4x4(const Matrix4x4& matrix, const char* label) {
	ImGui::Text("%s", label);
	if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
		// 
		for (int i = 0; i < 4; ++i) {
			ImGui::TableNextRow();
			for (int j = 0; j < 4; ++j) {
				ImGui::TableSetColumnIndex(j);
				ImGui::Text("%.3f", matrix.m[i][j]);
			}
		}
		ImGui::EndTable();
	}
}


bool GameScene::CheckCollision(Collider colliderA, Collider colliderB)
{
	colliderA.GetColliderType();
	colliderB.GetColliderType();

	if (colliderA.GetColliderType() == Collider::ColliderType::typeAABB &&
		colliderB.GetColliderType() == Collider::ColliderType::typeAABB)
	{
		return AABBtoAABBCollision(colliderA.GetAABB(), colliderB.GetAABB());
	}
	else if (colliderA.GetColliderType() == Collider::ColliderType::typeAABB &&
		colliderB.GetColliderType() == Collider::ColliderType::typeSphere)
	{
		return AABBSphereCollision(colliderA.GetAABB(), colliderB.GetSphere());
	}
	else if (colliderA.GetColliderType() == Collider::ColliderType::typeSphere &&
		colliderB.GetColliderType() == Collider::ColliderType::typeAABB)
	{
		return AABBSphereCollision(colliderB.GetAABB(), colliderA.GetSphere());
	}
}

void GameScene::Initialize()
{

	/*TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");*/

	//Audio::GetInstance()->SoundPlayWave("Resources/Alarm01.wav");

	//sprite = std::make_unique<Sprite>();

	//sprite->Initialize(SpriteCommon::GetInstance(), "Resources/uvChecker.png");

	//sprite->SetTexSize({ 512.0f,512.0f });

	ModelManager::GetInstance()->LoadModel("AnimatedCube/AnimatedCube.gltf");

	camera = std::make_unique<Camera>();
	camera->Initialize();

	object3d = std::make_unique<Object3d>();

	object3d->Initialize(Object3dCommon::GetInstance());

	object3d->SetModel("AnimatedCube/AnimatedCube.gltf");

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

	animationManager = std::make_unique<AnimationManager>();

	animationManager->LoadAnimationFile("./Resources/AnimatedCube", "AnimatedCube.gltf");

	animationManager->StartAnimation("AnimatedCube.gltf", 0);

	inputHandler_ = new InputHandler();
	inputHandler_->AssignMoveRightCommand2PressKeyD();
	inputHandler_->AssignMoveLeftCommand2PressKeyA();

	player_ = std::make_unique<Player>();
	player_->Initialize(ModelManager::GetInstance()->FindModel("AnimatedCube/AnimatedCube.gltf"));

	Enemy* enemy = new Enemy();
	enemy->Initialize(ModelManager::GetInstance()->FindModel("AnimatedCube/AnimatedCube.gltf"));

	enemy->SetTranslate({ 0.0f,0.0f,10.0f });

	enemys_.push_back(*enemy);
}

void GameScene::CheckAllCollision()
{
	for (auto bullet : player_->GetBullet())
	{
		for (auto it = enemys_.begin(); it != enemys_.end(); ) {
 			if (CheckCollision(bullet, *it))
			{
				bullet.OnCollision();
				it->OnCollision();
			}
			
			++it;         // 次のイテレータへ
		}
	}
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{

	camera->Update();

	iCommand_ = inputHandler_->HandleInput();

	player_->Update(camera->GetViewProjection());

	for (auto it = enemys_.begin(); it != enemys_.end(); ) {
		
		 // eraseは次のイテレータを返す
		if (!it->CheckIsAlive())
		{
			it = enemys_.erase(it);
		}
		else
		{
           it->Update(); // 更新処理を行う
	       ++it;   
		}
	         // 次のイテレータへ
	}


	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		player_->CheckFireInput();
	}

	if (this->iCommand_)
	{
		iCommand_->Exec(*player_);
	}

	

#ifdef _DEBUG
	
	if (ImGui::TreeNode("Object.transform")) {
		ImGui::DragFloat3("object.translate", &objectTransform->transform.translate.x, 0.01f);
		ImGui::DragFloat3("object.rotate", &objectTransform->transform.rotate.x, 0.01f);
		ImGui::DragFloat3("Object.scale", &objectTransform->transform.scale.x, 0.01f);
		ImGui::TreePop(); // TreeNodeを閉じる
	}

	if (ImGui::TreeNode("directionalLight")) {
		ImGui::ColorEdit4("directionalLight.color", &directionalLight->color_.x, 0.01f);
		if (ImGui::DragFloat3("directionalLight.direction", &directionalLight->direction_.x, 0.01f))
		{
			directionalLight->direction_ = Normalize(directionalLight->direction_);
		}
		ImGui::DragFloat("directionalLight.intensity", &directionalLight->intensity_, 0.01f);
		ImGui::TreePop(); // TreeNodeを閉じる
	}

	if (ImGui::TreeNode("pointLight")) {
		ImGui::ColorEdit4("pointLight.color", &pointLight->color_.x, 0.01f);
		ImGui::DragFloat3("pointLight.position", &pointLight->position_.x, 0.01f);
		ImGui::DragFloat("pointLight.decay", &pointLight->decay_, 0.01f);
		ImGui::DragFloat("pointLight.radius", &pointLight->radius_, 0.01f);
		ImGui::DragFloat("pointLight.intensity", &pointLight->intensity_, 0.01f);
		ImGui::TreePop(); // TreeNodeを閉じる
	}

	if (ImGui::TreeNode("spotLight")) {
		ImGui::ColorEdit4("spotlLight.color", &spotLight->color_.x, 0.01f);
		if (ImGui::DragFloat3("spotLight.direction", &spotLight->direction_.x, 0.01f))
		{
			spotLight->direction_ = Normalize(spotLight->direction_);
		}
		ImGui::DragFloat3("spotLight.position", &spotLight->position_.x, 0.01f);
		ImGui::DragFloat("spotLight.decay", &spotLight->decay_, 0.01f);
		ImGui::DragFloat("spotLight.intensity", &spotLight->intensity_, 0.01f);
		ImGui::TreePop(); // TreeNodeを閉じる
	}


	Matrix4x4 localMatrix = animationManager->GetLocalMatrix();

	ShowMatrix4x4(localMatrix, "localMatrix");

	
	ShowMatrix4x4(objectTransform->matWorld_, "worldMatrix");
#endif

	animationManager->Update();
    objectTransform->UpdateMatrix();
	object3d->Update();
	object3d->SetLocalMatrix(animationManager->GetLocalMatrix());

	if (Input::GetInstance()->Triggerkey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	pointLight->Update();

	spotLight->Update();

	CheckAllCollision();

	Vector3 reticlePosition = player_->Get2DReticlePosition();

	for (auto enemy : enemys_)
	{
		enemy.GetWorldTransform();


		WorldTransform reticle;

		reticle.Initialize();
		reticle.transform.translate = enemy.GetWorldTransform().transform.translate;
		reticle.UpdateMatrix();

		Vector3 enemyPosition = { reticle.matWorld_.m[3][0],reticle.matWorld_.m[3][1], reticle.matWorld_.m[3][2] };

		// スクリーン座標に変換
		

		Matrix4x4 matViewport = MakeViewportMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 1.0f);

		Matrix4x4 matViewProjectionViewPort = Multiply(Multiply(camera->GetViewProjection().matView_, camera->GetViewProjection().matProjection_), matViewport);

		enemyPosition = MakeTransform(enemyPosition, matViewProjectionViewPort);

		if (isLockOn_)
		{

		}
		else
		{

		}

	}
}

void GameScene::Draw()
{

	SpriteCommon::GetInstance()->SetView();

	//sprite->Draw();

	Object3dCommon::GetInstance()->SetView();

	object3d->Draw(player_->GetWorldTransform(), Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	player_->Draw(Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	for (auto it = enemys_.begin(); it != enemys_.end(); ) 
	{
		it->Draw(Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get()); 
		++it;         // 次のイテレータへ
	}
}
