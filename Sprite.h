#pragma once
#include "wrl.h"
#include "d3d12.h"
#include "Vectors.h"
#include "MyMath.h"
#include <cstdint>
#include <string>

class SpriteCommon;

class Sprite
{
public: // メンバ関数
	// 初期化
	void Initialize(SpriteCommon* spriteCommon,std::string textureFilePath);

	void Update();

	void Draw();

	const Vector2& GetPosition() const { return position;}

	void SetPosition(const Vector2& position) { this->position = position; }
	
	float GetRotation() const { return rotation; }
	
	void SetRotation(float rotation) { this->rotation = rotation; }

	const Vector4 GetColor() const { return materialData->color; }

	void SetColor(const Vector4& color) { materialData->color = color; }
	
	const Vector2& GetSize() const { return size; }

	void SetSize(const Vector2& size) { this->size = size; }

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

	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Transform uvTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	Vector2 position = { 0.0f,0.0f };

	float rotation = 0.0f;

	Vector2 size = { 640.0f,360.0f };

	// テクスチャ番号
	uint32_t textureIndex = 0;
};

