#pragma once
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#include <XInput.h>
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include<cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace Microsoft::WRL;

// 入力
class Input
{
public: // メンバ変数
	// 初期化
	void Initialize(HINSTANCE hInstance,HWND hwnd);

	// 更新
	void Update();

private:
};

