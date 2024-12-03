#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"
#include "MyMath.h"

#include "DirectXCommon.h"

struct SpotLightData
{
	Vector4 color; // ライトの色
	Vector3 position; // ライトの位置
	float intensity; // 輝度
	Vector3 direction; // ライトの方向
	float distance; // ライトの届く最大距離
	float decay; // 減衰率
	float cosAngle; // スポットライトの余弦
	float cosFalloffStart;
	float padding[2];
};

class SpotLight
{
private:

	

public:
	void Initialize();

	void Update();

	SpotLightData& GetData() { return spotLightData_; }

public:

	Vector4 color_;
	Vector3 position_;
	float intensity_;
	Vector3 direction_;
	float distance_;
	float decay_;
	float cosAngle_;
	float cosFalloffStart_;

private:

	SpotLightData spotLightData_;
};