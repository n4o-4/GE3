﻿#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>

#include "Logger.h"
#include "StringUtility.h"
#include "WinApp.h"
#include <dxcapi.h>
#include <format>

#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"


class DirectXCommon
{
public:
	void Initialize(WinApp* winApp);

	// デバイス初期化
	void InitializeDevice();

	// コマンド関連の初期化
	void InitializeCommands();

	// スワップチェーンの生成
	void CreateSwapChain();

	// 深度バッファの生成
	void CreateDepthBuffer();

	// 各種出スクリプターヒープの生成
	void CreateDescriptorHeaps();

	// レンダーターゲットビューの初期化
	void InitializeRenderTergetView();

	// 深度ステンシルビューの初期化
	void InitializeDepthStencilView();

	// フェンスの生成
	void InitializeFence();

	// ビューポート矩形の初期化
	void InitializeViewPort();

	// シザリング矩形
	void Scissor();

	// DXCコンパイラーの生成
	void CreateDXCCompiler();

	// ImGuiの初期化
	void InitializeImGui();

	//
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
	{
		//ID3D12DescriptorHeap* descriptorHeap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
		D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
		descriptorHeapDesc.Type = heapType;
		descriptorHeapDesc.NumDescriptors = numDescriptors;
		descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
		assert(SUCCEEDED(hr));
		return descriptorHeap;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, int32_t width, int32_t height)
	{
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Width = width;        // textureの幅
		resourceDesc.Height = height;      // textureの高さ
		resourceDesc.MipLevels = 1;        // mipmapの数
		resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
		resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
		resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定。
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStrncilとして使う通知

		// 利用するHeapの設定
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

		// 深度値のクリア設定
		D3D12_CLEAR_VALUE depthClearValue{};
		depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
		depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

		// Resourceの生成
		//ID3D12Resource* resource = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		HRESULT hr = device->CreateCommittedResource(
			&heapProperties, // Heapの設定
			D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし
			&resourceDesc, // Resourceの設定
			D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
			&depthClearValue, // Clear最適
			IID_PPV_ARGS(&resource));
		assert(SUCCEEDED(hr));
		return resource;
	}

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		handleCPU.ptr += (descriptorSize * index);
		return handleCPU;
	}

	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {
		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart();
		handleGPU.ptr += (descriptorSize * index);
		return handleGPU;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

private:
	// WindowAPI
	WinApp* winApp = nullptr;

	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;

	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;

	Microsoft::WRL::ComPtr<IDXGISwapChain4>  swapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;

	uint32_t descriptorSizeSRV = 0;

	uint32_t descriptorSizeRTV = 0;

	uint32_t descriptorSizeDSV = 0;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;

	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> srvDescriptorHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2] = { nullptr };

	Microsoft::WRL::ComPtr< ID3D12Fence> fence = nullptr;

	D3D12_VIEWPORT viewport{};

	D3D12_RECT scissorRect{};

	IDxcUtils* dxcUtils = nullptr;

	IDxcCompiler3* dxcCompiler = nullptr;
};
