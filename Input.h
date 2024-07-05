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
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ変数
	// 初期化
	void Initialize(HINSTANCE hInstance,HWND hwnd);

	// 更新
	void Update();

	bool PushKey(BYTE keyNumber);

	bool Triggerkey(BYTE keyNumber);
private:
	// キーボードデバイス
	ComPtr<IDirectInputDevice8> keyboard;

	ComPtr<IDirectInput8> directInput;

	BYTE key[256] = {};

	BYTE keyPre[256] = {};
};

