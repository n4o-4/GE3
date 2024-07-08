#include <string>
#include <stdio.h>
#include <format>
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

#include "Audio.h"


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
	D3D12ResourceLeakChecker leakChecker;

	HRESULT hr;
#pragma region 基盤システムの初期化

	/*-----------------------------------
	* WinAppの生成と初期化
	-----------------------------------*/
	std::unique_ptr<WinApp> winApp = nullptr;
	winApp = std::make_unique<WinApp>();
	winApp->Initialize();

	/*-----------------------------------
	* Inputの生成と初期化
	-----------------------------------*/

	std::unique_ptr<Input> input = nullptr;
	// 入力の初期化
	input = std::make_unique<Input>();
	input->Initialize(winApp.get());

	/*-----------------------------------
	* DirectXCommonの生成と初期化
	-----------------------------------*/

	std::unique_ptr<DirectXCommon> dxCommon = nullptr;
	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());
	
	/*-----------------------------------
	* Audioの生成と初期化
	-----------------------------------*/

	std::unique_ptr<Audio> audio = nullptr;
	audio = std::make_unique<Audio>();
	audio->Initialize();


	audio->SoundLoadWave("Resources/Alarm01.wav");
	audio->SoundPlayWave();

	/*-----------------------------------
	* SrvManagerの生成と初期化
	-----------------------------------*/
	std::unique_ptr<SrvManager> srvManager = nullptr;
	srvManager = std::make_unique<SrvManager>();
	srvManager->Initialize(dxCommon.get());

	/*-----------------------------------
	* TextureManagerの初期化
	-----------------------------------*/

	TextureManager::GetInstance()->Initialize(dxCommon.get(), srvManager.get());

	/*-----------------------------------
	* SpriteCommonの生成と初期化
	* 
	*  スプライト共通部
	-----------------------------------*/

	std::unique_ptr<SpriteCommon> spriteCommon = nullptr;
	spriteCommon = std::make_unique<SpriteCommon>();
	spriteCommon->Initialize(dxCommon.get());
	
	/*-----------------------------------
	* Object3dCommonの生成と初期化
	* 
	*  3Dオブジェクト共通部
	-----------------------------------*/

	std::unique_ptr<Object3dCommon> object3dCommon = nullptr;
	object3dCommon = std::make_unique<Object3dCommon>();
	object3dCommon->Initialize(dxCommon.get());

	/*-----------------------------------
	* ModelCommonの生成と初期化
	* 
	*  モデル共通部
	-----------------------------------*/

	std::unique_ptr<ModelCommon> modelCommon = nullptr;
	modelCommon = std::make_unique<ModelCommon>();
	modelCommon->Initialize(dxCommon.get());

	/*-----------------------------------
	* ModelManagerの初期化
	-----------------------------------*/

	ModelManager::GetInstance()->Initialize(dxCommon.get());

	/*-----------------------------------
	* Cameraの初期化
	-----------------------------------*/

	std::unique_ptr<Camera> camera = nullptr;
	camera = std::make_unique<Camera>();

	/*-----------------------------------
	* ParticleManagerの初期化
	-----------------------------------*/

	ParticleManager::GetInstance()->Initialize(dxCommon.get(), srvManager.get(), camera.get());
	ParticleManager::GetInstance()->CreateParticleGroup("particle", "Resources/circle.png");

	/*-----------------------------------
	* ParticleEmitterの生成と初期化
	-----------------------------------*/

	std::unique_ptr<ParticleEmitter> particleEmitter = nullptr;
	particleEmitter = std::make_unique<ParticleEmitter>();
	particleEmitter->Initialize("particle");
	particleEmitter->Emit();

#ifdef _DEBUG

	std::unique_ptr<ImGuiManager> imGuiManager = nullptr;
	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp.get(), dxCommon.get());

#endif

#pragma endregion 基盤システム初期化

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	//std::vector<Sprite*> sprites;
	std::unique_ptr<Sprite> sprite = nullptr;
	sprite = std::make_unique<Sprite>();
	sprite->Initialize(spriteCommon.get(), "Resources/uvChecker.png");
	//sprites.push_back(sprite);


	//object3dCommon->SetDefaultCamera(camera.get());

	//Model* model = new Model();
	//model->Initialize(modelCommon.get(), "Resources", "plane.obj");

	//Object3d* object3d = new Object3d();
	//object3d->Initialize(object3dCommon.get());
	//object3d->SetModel(model);

	////
	//ModelManager::GetInstance()->LoadModel("axis.obj");

	//Object3d* object3d1 = new Object3d();
	//object3d1->Initialize(object3dCommon.get());
	//object3d1->SetModel("axis.obj");


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

			imGuiManager->Draw(dxCommon.get());

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

	ParticleManager::GetInstance()->Finalize();

	ModelManager::GetInstance()->Finalize();

	TextureManager::GetInstance()->Finalize();

	//delete object3d;
	//delete object3dCommon;

	//delete srvManager;
	//delete dxCommon;

	audio->SoundUnload();

	winApp->Finalize();
	//delete winApp;
	//delete input;


	return 0;
}