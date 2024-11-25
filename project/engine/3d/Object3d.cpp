#include "Object3d.h"
#include "MyMath.h"
#include "TextureManager.h"

#include "imgui.h"

void Object3d::Initialize(Object3dCommon* object3dCommon)
{
	this->object3dCommon = object3dCommon;

	transformationResource = this->object3dCommon->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	// データを書き込むためのアドレスを取得
	transformationResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	//　単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();

	directionLightResource = this->object3dCommon->GetDxCommon()->CreateBufferResource(sizeof(DirectionLight));
	directionLightResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionLightData));

	directionLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionLightData->direction = { 1.0f,0.0f,0.0f };
	directionLightData->intensity = 1.0f;


	this->camera = object3dCommon->GetDefaultCamera();

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Object3d::Update()
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix;

#ifdef _DEBUG

	//ImGui::DragFloat3("light.Direction", &directionLightData->direction.x);

	if (ImGui::DragFloat3("light.Direction", &directionLightData->direction.x,0.01f)) {
		// Normalize the direction vector
		Vector3& dir = directionLightData->direction;
		float length = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
		if (length > 0.0f) {
			dir.x /= length;
			dir.y /= length;
			dir.z /= length;
		}
	}

	ImGui::DragFloat("light.intensity", &directionLightData->intensity, 0.01f);


#endif

	if (camera) {
		
	}
	else {

		worldViewProjectionMatrix = worldMatrix;
	}
	//transformationMatrixData->WVP = worldViewProjectionMatrix;
}

void Object3d::Draw(WorldTransform worldTransform,ViewProjection viewProjection,PointLight pointLight)
{
	object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, viewProjection.GetViewProjectionResource()->GetGPUVirtualAddress());
	
	object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionLightResource.Get()->GetGPUVirtualAddress());

	object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, worldTransform.GetTransformResource()->GetGPUVirtualAddress());

	object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLight.GetPointLightResource()->GetGPUVirtualAddress());

	if (model) {
		model->Draw();
	}
}

void Object3d::SetModel(const std::string& filePath)
{
	model = ModelManager::GetInstance()->FindModel(filePath);
}


