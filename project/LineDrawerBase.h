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
		int padding[3];
	};

	struct LineForGPU
	{
		Matrix4x4 matWorld;
		Vector4 position[2];
		Vector4 color;
	};

	struct LineObject
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
		VertexData* vertexData = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;
		uint32_t* indexData = nullptr;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;

		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource = nullptr;
		LineForGPU* instancingData = nullptr;
		
		int32_t srvIndex;
	};

public: // メンバ関数

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

	// 更新
	void Update();
	
	// 描画
	void Draw(ViewProjection viewProjection);

private: // メンバ関数

	void CreateLineResource();

	void CreateRootSignature();

	void CreatePipellineState();

	void CreateLineObject();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateVertexResource();

	void CreateVertexBufferView(LineObject* object);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateIndexResource();

	void CreateIndexBufferView(LineObject* object);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateInstancingResource();

	

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	std::unique_ptr<Pipeline> pipeline_ = nullptr;

	

	//std::unique_ptr<LineObject> lineObject_ = nullptr;

	std::list<std::unique_ptr<LineObject>> lineObjects_;

	WorldTransform transform;

	uint32_t instanceNum = 0;
};

