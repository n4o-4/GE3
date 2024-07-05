#pragma once
#include <Windows.h>
#include <cstdint>
#include "externals/imgui/imgui.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


class WinApp
{
public: // �ÓI�����o�֐�
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	void Initialize();

	
	HWND GetHWND() const { return hwnd; }

	HINSTANCE GetInstance() const { return wc.hInstance; }

public: // �萔
	// �N���C�A���g�̈�̃T�C�Y
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:
	HWND hwnd = nullptr;

	WNDCLASS wc{};
};

