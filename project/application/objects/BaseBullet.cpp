#include "BaseBullet.h"

void BaseBullet::Initialize(Object3dCommon* object3dCommon,Vector3 initialPosition)
{
	Collider::Initialize(ColliderType::typeSphere);
	Collider::SetRadius(1.0f);

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(object3dCommon);
	//worldTransform_ = new WorldTransform();
	worldTransform.Initialize();
	worldTransform.transform.translate = initialPosition;
	velocity_ = { 0.0f,0.0f,0.5f };
	object3d_->SetLocalMatrix(MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }));

	spawnTime_ = std::chrono::steady_clock::now();

	bool isAlive_ = true;
}

void BaseBullet::SetModel(Model* model)
{
	object3d_->SetModel(model);
}

void BaseBullet::Update()
{
	worldTransform.transform.translate += velocity_;
    worldTransform.UpdateMatrix();
    Collider::Update();

 	

	object3d_->Update();
}

void BaseBullet::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	object3d_->Draw(worldTransform, viewProjection, directionalLight, pointLight, spotLight);
}

void BaseBullet::OnCollision()
{
	isAlive_ = false;
}
