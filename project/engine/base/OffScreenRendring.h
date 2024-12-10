#pragma once
#include "DirectXCommon.h"

class OffScreenRendring
{
/// --------------------------------
/// メンバ関数
///---------------------------------
public:
	void Initialzie();
	void PreDraw();
	void PostDraw();

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	void CreateRenderTextureRTV();


/// --------------------------------
/// メンバ変数
///---------------------------------
public:

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResources = nullptr ;

	D3D12_RESOURCE_BARRIER barrier{};
};