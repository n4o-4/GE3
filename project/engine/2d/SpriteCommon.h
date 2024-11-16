#pragma once
#include "DirectXCommon.h"

class SpriteCommon
{
public: // メンバ関数

	// シングルトンインスタンスの取得
	static SpriteCommon* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	void SetView();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:

	static SpriteCommon* instance;

	SpriteCommon() = default;
	~SpriteCommon() = default;
	SpriteCommon(SpriteCommon&) = delete;
	SpriteCommon& operator=(SpriteCommon&) = delete;

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