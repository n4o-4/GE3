#include "TitleScene.h"

void ShowMatrix4x4(const Matrix4x4& matrix, const char* label) {
	ImGui::Text("%s",label);
	if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
		// äeçsÇï`âÊ
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

	Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
	Vector3 to0 = Normalize(from0 * -1.0f);
	Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
	Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });

	rotateMatrix0 = DirectionToDirection(Normalize(Vector3{ 1.0f,0.0f,0.0f }), Normalize(Vector3{ -1.0f,0.0f,0.0f }));
	rotateMatrix1 = DirectionToDirection(from0, to0);
	rotateMatrix2 = DirectionToDirection(from1, to1);
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

#ifdef _DEBUG

	ImGui::Begin("rotateMatrix");

	ShowMatrix4x4(rotateMatrix0, "rotateMatrix0");
	ShowMatrix4x4(rotateMatrix1, "rotateMatrix1");
	ShowMatrix4x4(rotateMatrix2, "rotateMatrix2");

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
