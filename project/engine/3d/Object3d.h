#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include <fstream>
#include <string>
#include "Object3dCommon.h"
#include "Model.h"
#include "WinApp.h"
#include "ModelManager.h"
#include "Camera.h"

class Object3d
{
private:

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};


public: // メンバ関数
	// 初期化
	void Initialize(Object3dCommon* object3dCommon);

	// 更新
	void Update();

	// 描画
	void Draw();

	void SetModel(const std::string& filePath);

	// setter
	void SetScale(const Vector3& scale) { transform.scale = scale; }
	void SetRotation(const Vector3& rotation) { transform.rotate = rotation; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }

	const Vector3& GetScale() { return transform.scale; }
	const Vector3& GetRotation() { return transform.rotate;}
	const Vector3& GetTranslate() { return transform.translate; }

	void SetCamera(Camera* camera) { this->camera = camera; }

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
	//Transform cameraTransform;

	Model* model = nullptr;

	Camera* camera = nullptr;
};

