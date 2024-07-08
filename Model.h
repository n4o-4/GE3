#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include "ModelCommon.h"
#include <fstream>
#include <string>

// 3Dモデル
class Model
{
public:
	// 初期化
	void Initialize(ModelCommon* modelCommon);

	// 描画
	void Draw();

public:

	inline struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	inline struct MaterialData
	{
		std::string textureFilePath;
		uint32_t textureIndex;
	};

	inline struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	inline struct Material {
		Vector4 color;
		int enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

private:

	// .mtlファイルの読み込み
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	ModelCommon* modelCommon_;

	ModelData modelData;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

	Material* materialData = nullptr;
};

