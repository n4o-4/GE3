#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

#include "MyMath.h"

struct ConstBufferDataViewProjection
{
	Matrix4x4 matView;
	Matrix4x4 matProjection;
};

class ViewProjection
{
public:

	void Initialize();

	void Update();

	Microsoft::WRL::ComPtr<ID3D12Resource> &GetViewProjectionResource() { return viewProjectionResource_; }

public:

	Transform transform;

	Matrix4x4 matView_;
	Matrix4x4 matProjection_;

	float fovY;
	float aspectRation;
	float nearClip;
	float farClip;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> viewProjectionResource_;

	ConstBufferDataViewProjection* viewProjectionData_;
};
