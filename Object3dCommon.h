#pragma once
#include "DirectXCommon.h"

class Object3dCommon
{
public: // メンバ関数
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// 共通描画設定
	void SetView();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:
	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();

	// ルートシグネチャの生成
	void CreateRootSignature();

private: // メンバ変数
	DirectXCommon* dxCommon_;

    Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	//ID3DBlob* signatureBlob = nullptr;
	Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;;
};

