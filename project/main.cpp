//#include <string>
//#include <stdio.h>
//#include <format>
//#include <cassert>
//#include <dxgidebug.h>
//#include <debugapi.h>
//#include "externals/DirectXTex/DirectXTex.h"
//#define _USE_MATH_DEFINES
//#include <math.h>
//#include <fstream>
//#include <sstream>
//#include "Input.h"
//#include "WinApp.h"
//#include "DirectXCommon.h"
//#include "SpriteCommon.h"
//#include "Sprite.h"
//#include "MyMath.h"
//#include "Vectors.h"
//#include "Matrixs.h"
//#include "TextureManager.h"
//#include "Object3dCommon.h"
//#include "Object3d.h"
//#include "Model.h"
//#include "ModelCommon.h"
//#include "ModelManager.h"
//#include "Camera.h"
//#include "SrvManager.h"
//#include "Logger.h"
//
//#include "ParticleManager.h"
//#include "ParticleEmitter.h"
//
//#include "ImGuiManager.h"
//
//#include "Audio.h"

#include"MyGame.h"

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

	std::unique_ptr<MyGame> myGame = std::make_unique<MyGame>();

	myGame->Initialize();

	while (true)
	{
		

		myGame->Update();

		if (myGame->IsEndRequest())
		{
			break;
		}

	}

	myGame->Finalize();

	return 0;
}