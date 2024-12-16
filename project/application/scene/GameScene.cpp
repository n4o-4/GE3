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
	else if (colliderA.GetColliderType() == Collider::ColliderType::typeSphere &&
		colliderB.GetColliderType() == Collider::ColliderType::typeSphere)
	{
		return SphereCollision(colliderA.GetSphere(),colliderB.GetSphere());
	}

	return false;
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

	ModelManager::GetInstance()->LoadModel("AL4_Player.obj");
	ModelManager::GetInstance()->LoadModel("ass_Player_Bullet.obj");
	ModelManager::GetInstance()->LoadModel("AL4_Skydome.obj");
	ModelManager::GetInstance()->LoadModel("Space.obj");
	ModelManager::GetInstance()->LoadModel("GameClear.obj");

	camera = std::make_unique<Camera>();
	camera->Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/reticle.png");
	TextureManager::GetInstance()->LoadTexture("Resources/reticle_lockOn.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Space.png");

	reticle = std::make_unique<Sprite>();
	lockOn_Reticle = std::make_unique<Sprite>();

	reticle->Initialize(SpriteCommon::GetInstance(), "Resources/reticle.png");
	lockOn_Reticle->Initialize(SpriteCommon::GetInstance(), "Resources/reticle_lockOn.png");

	reticle->SetSize({ 64.0f,64.0f });
	lockOn_Reticle->SetSize({ 64.0f,64.0f });

	reticle->SetTexSize({ 64.0f,64.0f });
	lockOn_Reticle->SetTexSize({ 64.0f,64.0f });

	reticle->SetAnchorPoint({ 0.5f,0.5f });
	lockOn_Reticle->SetAnchorPoint({ 0.5f,0.5f });

	object3d = std::make_unique<Object3d>();

	object3d->Initialize(Object3dCommon::GetInstance());

	object3d->SetModel("AL4_Player.obj");

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

	//animationManager = std::make_unique<AnimationManager>();

	//animationManager->LoadAnimationFile("./Resources/AnimatedCube", "AnimatedCube.gltf");

	//animationManager->StartAnimation("AnimatedCube.gltf", 0);

	inputHandler_ = new InputHandler();
	inputHandler_->AssignMoveRightCommand2PressKeyD();
	inputHandler_->AssignMoveLeftCommand2PressKeyA();

	player_ = std::make_unique<Player>();
	player_->Initialize(ModelManager::GetInstance()->FindModel("ass_Player_Bullet.obj"));

	for (int i = 0; i < 3; ++i)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->Initialize(ModelManager::GetInstance()->FindModel("AL4_Player.obj"), ModelManager::GetInstance()->FindModel("ass_Player_Bullet.obj"));

		enemy->SetTranslate({ i * 5.0f - 5.0f,0.0f,50.0f });

		enemys_.push_back(std::move(enemy));
	}

	ModelManager::GetInstance()->FindModel("AL4_Player.obj")->SetEnableLighting(false);

	skydome_ = std::make_unique<Object3d>();
	skydome_->Initialize(Object3dCommon::GetInstance());
	skydome_->SetModel(ModelManager::GetInstance()->FindModel("AL4_Skydome.obj"));
	ModelManager::GetInstance()->FindModel("AL4_Skydome.obj")->SetEnableLighting(false);

	skydomeTransform_ = std::make_unique<WorldTransform>();
	skydomeTransform_->Initialize();



	space = std::make_unique<Object3d>();
	space->Initialize(Object3dCommon::GetInstance());
	space->SetModel(ModelManager::GetInstance()->FindModel("Space.obj"));

	spaceTrans_ = std::make_unique<WorldTransform>();
	spaceTrans_->Initialize();

	spaceTrans_->transform.rotate = { 1.74f,3.14f,0.0f };
	spaceTrans_->transform.translate = { 0.0f,-1.0f,0.0f };

	isPushSpace = false;
}

void GameScene::CheckAllCollision()
{
#pragma region ----自機の弾と敵の当たり判定----

	for (auto bullet = player_->GetBullet().begin(); bullet != player_->GetBullet().end(); ++bullet)
	{
		for (auto it = enemys_.begin(); it != enemys_.end(); ) {
			if (CheckCollision(*bullet->get(), *it->get()))
			{
				bullet->get()->OnCollision();
				it->get()->OnCollision();
			}

			++it;         // 次のイテレータへ
		}
	}	

#pragma endregion ----自機の弾と敵の当たり判定----

#pragma region 敵のの弾と自機の当たり判定

	for (auto it = enemys_.begin(); it != enemys_.end(); ) {
		
		for (auto bullet = it->get()->GetBullet().begin(); bullet != it->get()->GetBullet().end(); ++bullet)
		{
			if (CheckCollision(*bullet->get(), *player_.get()))
			{
				bullet->get()->OnCollision();
				player_->OnCollision();
			}
		}

		++it;         // 次のイテレータへ
	}

#pragma endregion ----敵の弾と自機の当たり判定----

#pragma region 自機の弾と敵の弾の当たり判定

	for (auto enemy = enemys_.begin(); enemy != enemys_.end(); ) {

		for (auto eBullet = enemy->get()->GetBullet().begin(); eBullet != enemy->get()->GetBullet().end(); ++eBullet)
		{
			
			for (auto pBullet = player_->GetBullet().begin(); pBullet != player_->GetBullet().end(); ++pBullet)
			{

				if (CheckCollision(*eBullet->get(), *pBullet->get()))
				{
					eBullet->get()->OnCollision();
					pBullet->get()->OnCollision();

					return;
				}
			}
		}
		++enemy;         // 次のイテレータへ
	}
#pragma endregion ----自機の弾と敵の弾の当たり判定----
}

// レティクルの計算関数
void GameScene::CalculationReticle()
{
	// 自機でのレティクルの位置を取得
	Vector3 reticlePosition = player_->Get2DReticlePosition();

	// レティクルの描画位置に自機での値を代入
	drawReticlrPosition = { reticlePosition.x,reticlePosition.y };

	// 
    player_->SetIsLockOn(false);

	// 敵の数分for文を回す
	for (auto it = enemys_.begin(); it != enemys_.end(); ) {
		it->get()->GetWorldTransform();


		WorldTransform reticle;

		reticle.Initialize();
		reticle.transform.translate = it->get()->GetWorldTransform().transform.translate;
		reticle.UpdateMatrix();

		Vector3 enemyPosition = { reticle.matWorld_.m[3][0],reticle.matWorld_.m[3][1], reticle.matWorld_.m[3][2] };

		Matrix4x4 matViewport = MakeViewportMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 1.0f);

		Matrix4x4 matViewProjectionViewPort = Multiply(Multiply(camera->GetViewProjection().matView_, camera->GetViewProjection().matProjection_), matViewport);

		enemyPosition = MakeTransform(enemyPosition, matViewProjectionViewPort);

		float length = sqrtf((reticlePosition.x - enemyPosition.x) * (reticlePosition.x - enemyPosition.x) + (reticlePosition.y - enemyPosition.y) * (reticlePosition.y - enemyPosition.y));

		if (length < 50.0f)
		{
			drawReticlrPosition = { enemyPosition.x,enemyPosition.y };

			player_->SetIsLockOn(true);

			player_->SetTargetPosition(it->get()->GetWorldTransform().transform.translate);

			break;
		}

		
		++it;
	}
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE) && isPushSpace == false)
	{
		isPushSpace = true;
	}

	camera->Update();

	iCommand_ = inputHandler_->HandleInput();

	player_->Update(camera->GetViewProjection());

    CalculationReticle();

	for (auto it = enemys_.begin(); it != enemys_.end(); ) {
		
		 // eraseは次のイテレータを返す
		if (!it->get()->CheckIsAlive())
		{	
			it = enemys_.erase(it);
		}
		else
		{
           it->get()->Update(player_->GetWorldTransform().transform.translate); // 更新処理を行う
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

	//object3d->SetLocalMatrix(animationManager->GetLocalMatrix());

	//animationManager->Update();
	
	if (Input::GetInstance()->Triggerkey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	if (!player_->GetIsAlive())
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	pointLight->Update();

	spotLight->Update();

	CheckAllCollision();

	skydomeTransform_->UpdateMatrix();

	spaceTrans_->UpdateMatrix();

	if (enemys_.begin() == enemys_.end())
	{
		isClear_ = true;
		space->SetModel(ModelManager::GetInstance()->FindModel("GameClear.obj"));
		spaceTrans_->transform.translate = { 0.0f,0.0f,0.0f };
	}

	if (isClear_)
	{
		++Count;

		if (Count >= 240)
		{
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}
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

	ShowMatrix4x4(objectTransform->matWorld_, "worldMatrix");

	int index = 0;

	for (auto it = enemys_.begin(); it != enemys_.end(); ) {

		Vector3 translate = it->get()->GetTranslate();

		ImGui::DragFloat3("enemy",&translate.x, 0.01f);

		it++;
	}

	for (auto playerBullet = player_->GetBullet().begin(); playerBullet != player_->GetBullet().end(); ++playerBullet)
	{
		Vector3 translate = playerBullet->get()->GetTranslate();

		ImGui::DragFloat3("Bullet", &translate.x, 0.01f);


	}

#endif

}

void GameScene::Draw()
{

	SpriteCommon::GetInstance()->SetView();

	//sprite->Draw();

	if (player_->GetIsLockOn())
	{
		lockOn_Reticle->SetPosition(drawReticlrPosition);
		lockOn_Reticle->Update();
		lockOn_Reticle->Draw();
	}
	else
	{
		reticle->SetPosition(drawReticlrPosition);
		reticle->Update();
		reticle->Draw();
	}

	Object3dCommon::GetInstance()->SetView();

	player_->Draw(Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	for (auto it = enemys_.begin(); it != enemys_.end(); ) 
	{
		it->get()->Draw(Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());
		++it;         // 次のイテレータへ
	}

	skydome_->Draw(*skydomeTransform_.get(), Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	if (!isPushSpace || isClear_)
	{
		space->Draw(*spaceTrans_.get(), Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());
	}

	SpriteCommon::GetInstance()->SetView();
	
	//sprite->Draw();
	
	if (player_->GetIsLockOn())
	{
		lockOn_Reticle->SetPosition(drawReticlrPosition);
		lockOn_Reticle->Draw();
	}
	else
	{
		reticle->SetPosition(drawReticlrPosition);
		reticle->Draw();
	}
}
