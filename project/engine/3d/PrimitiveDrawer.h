#pragma once
#include "DirectXCommon.h"
#include "Vectors.h"
#include "ViewProjection.h"
#include <d3d12.h>
#include <memory>
#include <string>
#include <wrl.h>

class PrimitiveDrawer
{
public:
	// 線分の最大数
	static const UINT kMaxLineCount = 4096;
	// 線分の頂点数
	static const UINT kVertexCountLine = 2;
	// 線分のインデックス数
	static const UINT kIndexCountLine = 0;

	struct LineData
	{
		Vector3 position[kMaxLineCount];
		Vector4 color;
	};

	struct LineDatas
	{
		LineData* lineData = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> lineResource_;
	};

///------------------------------
/// メンバ関数
///------------------------------
public:

	// 初期化
	void Initialize(DirectXCommon *dxCommon);

	// 更新
	void Update();

	// 描画
	void Draw();



private:

	// ルートシグネチャの生成
	void CreateRootSignature();

	// パイプラインの生成
	void CreatePipeLine();

	void CreateLineObjects();

///------------------------------
/// メンバ変数
///------------------------------
public:
private:
	//
	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	std::list<LineDatas> lineObjects_;
};