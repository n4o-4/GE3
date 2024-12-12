#include "TitleScene.h"

void ShowMatrix4x4(const Matrix4x4& matrix, const char* label) {
	ImGui::Text("%s",label);
	if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
		// �e�s��`��
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
	if (ImGui::BeginTable(label, 5, ImGuiTableFlags_Borders)) {
		// 1行目：ラベルとX, Y, Z, Wのヘッダー
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(label); // ラベルを最初の列に表示

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("X"); // Xのヘッダー

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("Y"); // Yのヘッダー

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("Z"); // Zのヘッダー

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("W"); // Wのヘッダー

		// 2行目：値を横並びで表示
		ImGui::TableNextRow(); // 新しい行
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(""); // ラベル列の値は空白

		ImGui::TableNextColumn();
		ImGui::Text("%.3f", quaternion.x); // X成分の値

		ImGui::TableNextColumn();
		ImGui::Text("%.3f", quaternion.y); // Y成分の値

		ImGui::TableNextColumn();
		ImGui::Text("%.3f", quaternion.z); // Z成分の値

		ImGui::TableNextColumn();
		ImGui::Text("%.3f", quaternion.w); // W成分の値

		ImGui::EndTable(); // テーブルを終了
	}
}

void ShowVector3(const Vector3& vector, const char* label)
{
	if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
		// 1行目：ラベルとX, Y, Zのヘッダー
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(label); // ラベルを最初の列に表示

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("X"); // Xのヘッダー

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("Y"); // Yのヘッダー

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("Z"); // Zのヘッダー

		// 2行目：値を横並びで表示
		ImGui::TableNextRow(); // 新しい行
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(""); // ラベル列の値は空白

		ImGui::TableNextColumn();
		ImGui::Text("%.3f", vector.x); // X成分の値

		ImGui::TableNextColumn();
		ImGui::Text("%.3f", vector.y); // Y成分の値

		ImGui::TableNextColumn();
		ImGui::Text("%.3f", vector.z); // Z成分の値

		ImGui::EndTable(); // テーブルを終了
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

	audio = std::make_unique<Audio>();
	audio->Initialize();
	audio->SoundPlay("Resources/Spinning_World.mp3",999);
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

	

	Quaternion rotate = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotate);
	Vector3 rotateByMatrix = fTransform(pointY, rotateMatrix);

#ifdef _DEBUG

	ImGui::Begin("Quaternion");

	ShowQuaternion(rotate, "rotate");
	ShowMatrix4x4(rotateMatrix, "rotateMatrix");
	ShowVector3(rotateByQuaternion, "rotateByQuaternion");
	ShowVector3(rotateByMatrix, "rotateByMAtrix");

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
