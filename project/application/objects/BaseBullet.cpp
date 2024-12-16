#include "BaseBullet.h"

void BaseBullet::Initialize(Object3dCommon* object3dCommon,Vector3 initialPosition)
{
	object3d_ = new Object3d();
	object3d_->Initialize(object3dCommon);
	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();
	worldTransform_->transform.translate = initialPosition;
	velocity_ = { 0.0f,0.0f,0.5f };
	object3d_->SetLocalMatrix(MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }));

	spawnTime_ = std::chrono::steady_clock::now();
}

void BaseBullet::SetModel(Model* model)
{
	object3d_->SetModel(model);
}

void BaseBullet::Update()
{
	worldTransform_->transform.translate += velocity_;

 	worldTransform_->UpdateMatrix();

	object3d_->Update();
}

void BaseBullet::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	object3d_->Draw(*worldTransform_, viewProjection, directionalLight, pointLight, spotLight);
}
