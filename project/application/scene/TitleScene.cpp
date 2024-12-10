#include "TitleScene.h"

void ShowMatrix4x4(const Matrix4x4& matrix, const char* label) {
	ImGui::Text("%s",label);
	if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
		// 各行を描画
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

void ShowQuaternion(const Quaternion& quaternion, const char* label)
{
	ImGui::Text("%s", label);
	// クォータニオンの成分を表示
	ImGui::Text("x: %.3f, y: %.3f, z: %.3f, w: %.3f", quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}



void TitleScene::Initialize()
{
	BaseScene::Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	sprite = std::make_unique<Sprite>();

	sprite->Initialize(SpriteCommon::GetInstance(), "Resources/monsterBall.png");

	sprite->SetTexSize({ 1200.0f,600.0f });

	sprite->SetAnchorPoint({ 0.5f,0.5f });

	sprite->SetPosition({ 640.0f,360.0f });

	sprite->SetSize({ 1280.0f,720.0f });

	ModelManager::GetInstance()->LoadModel("axis.obj");

	/*object3d = std::make_unique<Object3d>();

	object3d->Initialize(Object3dCommon::GetInstance());

	object3d->SetModel("axis.obj");

	object3d->SetCamera(camera.get());*/

	ParticleManager::GetInstance()->CreateParticleGroup("Particle_1", "Resources/circle.png");


	particleEmitter_1 = std::make_unique<ParticleEmitter>();
	particleEmitter_1->Initialize("Particle_1");
	particleEmitter_1->Emit();

	ParticleManager::GetInstance()->SetBlendMode("Add");
}

void TitleScene::Finalize()
{
	audio->SoundStop("Resources/Spinning_World.mp3");
}

void TitleScene::Update()
{
	if (Input::GetInstance()->Triggerkey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("GAME");

		return;
	}

	sprite->Update();

	/*Vector3 rotato = object3d->GetRotation();

	rotato.y += 0.02f;

	object3d->SetRotation(rotato);

	Vector3 translate = object3d->GetTranslate();

	translate.z += 0.1f;

	object3d->SetTranslate(translate);

	object3d->Update();*/

	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = qNormalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

#ifdef _DEBUG

	ImGui::Begin("Quaternion");

	ShowQuaternion(identity, "identity");
	ShowQuaternion(conj, "conjugate");
	ShowQuaternion(inv, "inverse");
	ShowQuaternion(normal, "Normal");
	ShowQuaternion(mul1, "multiply1");
	ShowQuaternion(mul2, "multiply2");

	ImGui::Text("norm = %.3f", norm);

	ImGui::End();
#endif // _DEBUG


	ParticleManager::GetInstance()->Update();
	particleEmitter_1->Update();
}

void TitleScene::Draw()
{

	Object3dCommon::GetInstance()->SetView();

	//object3d->Draw();

	SpriteCommon::GetInstance()->SetView();

	//sprite->Draw();

	//ParticleManager::GetInstance()->Draw("Resources/circle.png");
}
