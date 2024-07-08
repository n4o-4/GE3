#pragma once
#include "wrl.h"
#include "d3d12.h"
#include "Vectors.h"
#include "MyMath.h"
#include <cstdint>

class SpriteCommon;

class Sprite
{
public: // メンバ関数
	// 初期化
	void Initialize(SpriteCommon* spriteCommon);

	void Update();

	void Draw(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);

private: 

	void CreateVertexData();

	void CreateIndexData();

	void CreateMaterialData();

	void CreateTransformationMatrixData();

private:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material {
		Vector4 color;
		int enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	SpriteCommon* spriteCommon = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData;
	uint32_t* indexData;

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource = nullptr;

	TransformationMatrix* transformationMatrixData;

	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-15.0f} };

	Transform uvTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
};

