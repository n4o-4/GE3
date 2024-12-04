#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

#include "MyMath.h"

struct ConstBufferDataWorldTransform
{
	Matrix4x4 matWorld;
	Matrix4x4 matWorldInverse;
};



class WorldTransform
{
public:

	void Initialize();

	void UpdateMatrix();

	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetTransformResource() { return transformResource_; }

public:

	Transform transform;

	Matrix4x4 matWorld_;

	Matrix4x4 matWorldInverse_;

private:

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> transformResource_;

	ConstBufferDataWorldTransform* transformData_ = nullptr;

	const WorldTransform* parent_ = nullptr;

public:



};