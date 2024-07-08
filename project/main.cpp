#include <string>
#include <stdio.h>
//#include <format>
#include <cassert>
#include <dxgidebug.h>
#include <debugapi.h>
#include "externals/DirectXTex/DirectXTex.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <sstream>
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "MyMath.h"
#include "Vectors.h"
#include "Matrixs.h"
#include "TextureManager.h"
#include "Object3dCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "ModelCommon.h"
#include "ModelManager.h"
#include "Camera.h"
#include "SrvManager.h"
#include "Logger.h"

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "ImGuiManager.h"

class ResourceObject {
public:
	ResourceObject(ID3D12Resource* resource)
		:resource_(resource)
	{}

	~ResourceObject() {
		if (resource_) {
			resource_->Release();
		}
	}

	ID3D12Resource* Get() { return resource_; }
private:
	ID3D12Resource* resource_;
};

//

struct DirectionLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};



struct D3D12ResourceLeakChecker {
	~D3D12ResourceLeakChecker()
	{
		//
		Microsoft::WRL::ComPtr<IDXGIDebug> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//D3D12ResourceLeakChecker leakChecker;

	HRESULT hr;
#pragma region 基盤システムの初期化
	WinApp* winApp = nullptr;

	winApp = new WinApp();
	winApp->Initialize();

	// ポインタ
	Input* input = nullptr;

	// 入力の初期化
	input = new Input();
	input->Initialize(winApp);

	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	
	SrvManager* srvManager = nullptr;
	// SRVマネージャの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	TextureManager::GetInstance()->Initialize(dxCommon,srvManager);

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);
	
	Object3dCommon* object3dCommon = nullptr;
	
	// 3Dオブジェクト共通部の初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	ModelCommon* modelCommon = nullptr;

	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);

#ifdef _DEBUG

	ImGuiManager* imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp, dxCommon);

#endif


	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	ModelManager::GetInstance()->Initialize(dxCommon);


	std::vector<Sprite*> sprites;
	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon, "Resources/uvChecker.png");
	sprites.push_back(sprite);

	//
	Camera* camera = new Camera();

	ParticleManager::GetInstance()->Initialize(dxCommon, srvManager, camera);
	ParticleManager::GetInstance()->CreateParticleGroup("particle", "Resources/circle.png");

	ParticleEmitter* particleEmitter = new ParticleEmitter();


	particleEmitter->Initialize("particle");

	particleEmitter->Emit();


	/*camera->SetRotate({ 0.0f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,0.0f,0.0f });*/

	object3dCommon->SetDefaultCamera(camera);

	Model* model = new Model();
	model->Initialize(modelCommon, "Resources", "plane.obj");

	Object3d* object3d = new Object3d();
	object3d->Initialize(object3dCommon);
	object3d->SetModel(model);

	//
	ModelManager::GetInstance()->LoadModel("axis.obj");

	Object3d* object3d1 = new Object3d();
	object3d1->Initialize(object3dCommon);
	object3d1->SetModel("axis.obj");

#pragma endregion 基盤システム初期化
	//object3d1->SetModel(model);

	/*for (uint32_t i = 0; i < 5; ++i) {
		
		if (i % 2 == 0) {
			sprite->Initialize(spriteCommon, "Resources/monsterBall.png");
		}
		else {
			sprite->Initialize(spriteCommon, "Resources/uvChecker.png");
		}

		sprites.push_back(sprite);
	}*/


	/*Sprite* sprite = nullptr;
	sprite = new Sprite();
	sprite->Initialize(spriteCommon);*/

	

	//===================
	// directionlight
	//===================
	
	//ID3D12Resource* directionLightResource = CreateBufferResource(device, sizeof(DirectionLight));
	//ResourceObject directionLightResource = CreateBufferResource(device, sizeof(DirectionLight));
	Microsoft::WRL::ComPtr<ID3D12Resource> directionLightResource = dxCommon->CreateBufferResource(sizeof(DirectionLight));
	DirectionLight* directionLightData = nullptr;
	directionLightResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionLightData));
	directionLightData->color = {1.0f,1.0f,1.0f,1.0f };
	directionLightData->direction = { 1.0f,0.0f,0.0f };
	directionLightData->intensity = 1.0f;

	
	/*Transform transform{ {1.0f,1.0f,1.0f},{0.0f,-3.2f,0.0f},{0.0f,0.0f,0.0f} };

	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-15.0f} };*/
	bool useMonsterBall = true;

	Logger::Log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n\n");

	Vector2 position = {};

	float rotate = {};

	MSG msg{};
	while (true)
	{

		if (winApp->ProcessMessage()) {
			break;
		}
		else
		{
			//////更新処理

			/*ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();*/
			//ImGui::Checkbox("UseMonsterBall", &useMonsterBall);
			
#ifdef _DEBUG

			imGuiManager->Begin();

			ImGui::DragFloat2("position", &position.x, 0.1f);

			ImGui::DragFloat("rotate", &rotate, 0.1f);

			imGuiManager->End();

#endif

			
	
			Vector3 cameraTranslate = camera->GetTranslate();

			sprite->SetPosition(position);

			sprite->SetRotation(rotate);

			//cameraTranslate.y += 0.1f;

			camera->SetTranslate(cameraTranslate);

			dxCommon->PreDraw();

			srvManager->PreDraw();

			object3dCommon->SetView();

			camera->Update();

			/*Vector3 rotation = object3d->GetRotation();

			rotation = { rotation.x,rotation.y += 0.03f,rotation.z };

			object3d->SetRotation(rotation);

			object3d->SetTranslatte({ -2.0f,0.0f,0.0f });

			object3d->Update();
			
			object3d->Draw();*/

			/*particleEmitter->Update();
			ParticleManager::GetInstance()->Update();
			ParticleManager::GetInstance()->Draw("Resources/uvChecker.png");*/

			/*rotation = object3d1->GetRotation();

			rotation = { rotation.x,0.0f,rotation.z += 0.03f };

			object3d1->SetRotation(rotation);

			object3d1->SetTranslatte({ 2.0f,0.0f,0.0f });

			object3d1->Update();

			object3d1->Draw();*/

			

		    //sprite->SetTexLeftTop({ 64.0f,64.0f });

		    sprite->SetTexSize({ 512.0f,512.0f });

			spriteCommon->SetView();

			sprite->Update();
			
			sprite->Draw();
			
			
			int i = 0;


			//ImGui::Render();

#ifdef _DEBUG

			imGuiManager->Draw(dxCommon);

#endif

			dxCommon->PostDraw();

			
		}
	}

	

	///
	
	//ImGui_ImplDX12_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	//ImGui::DestroyContext();

	/// 

	//CloseHandle(fenceEvent);

	ModelManager::GetInstance()->Finalize();

	TextureManager::GetInstance()->Finalize();

	delete object3d;
	delete object3dCommon;

	delete srvManager;
	delete dxCommon;
	winApp->Finalize();
	delete winApp;
	delete input;


	return 0;
}