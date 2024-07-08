#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include <fstream>
#include <string>
#include "Object3dCommon.h"
#include "Model.h"
#include "WinApp.h"

class Object3d
{
public: // メンバ関数
	// 初期化
	void Initialize(Object3dCommon* object3dCommon);

	// 更新
	void Update();

	// 描画
	void Draw();

	// setter
	void SetScale(const Vector3& scale) { transform.scale = scale; }
	void SetRotation(const Vector3& rotation) { transform.rotate = rotation; }
	void SetTranslatte(const Vector3& translate) { transform.translate = translate; }

	const Vector3& GetScale() { return transform.scale; }
	const Vector3& GetRotation() { return transform.rotate;}
	const Vector3& GetTranslate() { return transform.translate; }

public:

	/*inline struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};*/

	/*inline struct MaterialData
	{
		std::string textureFilePath;
		uint32_t textureIndex;
	};*/

	/*inline struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};*/

	/*inline struct Material {
		Vector4 color;
		int enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};*/

	inline struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	inline struct DirectionLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	inline struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	// setter
	void SetModel(Model* model) { this->model = model; }

private:

	

private:
	Object3dCommon* object3dCommon = nullptr;

	//ModelData modelData;

	// バッファリソース
	/*Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

	Material* materialData = nullptr;*/

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource;

	TransformationMatrix* transformationMatrixData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> directionLightResource;;
	DirectionLight* directionLightData = nullptr;

	Transform transform;
	Transform cameraTransform;

	Model* model = nullptr;
};

