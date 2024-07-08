#pragma once
#include "DirectXCommon.h"

class SpriteCommon
{
public: // メンバ関数
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void SetView();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:
	// ルートシグネチャの生成
	void CreateRootSignature();

	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();

private:
	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	//ID3DBlob* signatureBlob = nullptr;
	Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;;
};