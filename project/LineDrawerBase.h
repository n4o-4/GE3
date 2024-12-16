#pragma once
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


class LineDrawerBase
{
private:

	static const int kMaxLines = 2048;

	struct Pipeline
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	};

	struct VertexData
	{
		Vector4 position;
		int vertexIndex;
	};

	struct Line
	{
		Vector4 position[2];
		Vector4 color;
	};

	struct LineObject
	{
		WorldTransform worldTransform;
		std::list<Line> lines;

	};

	struct LineForGPU
	{
		Matrix4x4 matWorld;
		Vector4 position[2];
		Vector4 color;
	};

	struct LineResource
	{
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		VertexData* vertexData;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

		LineForGPU* instancingData;
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
	};

public: // メンバ関数

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

	// 更新
	void Update();
	
	// 描画
	void Draw();

private: // メンバ関数

	void CreateLineResource();

	void CreateRootSignature();

	void CreatePipellineState();

	void CreateVertexResource();

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	std::unique_ptr<Pipeline> pipeline_ = nullptr;

	VertexData vertices[2] = {
		{0.0f,0.0f,0.0f,0},
		{0.0f,0.0f,1.0f,1}
	};

	std::unique_ptr<LineResource> lineResource_ = nullptr;

	std::list<LineObject> objects_;

	LineObject lineObject_;

	WorldTransform transform;
};

