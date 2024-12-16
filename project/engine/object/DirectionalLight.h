#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

struct DirectionalLightData {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

class DirectionalLight
{
private:

public:

	void Initilaize();

	void Update();

	DirectionalLightData& GetData() { return directionalLightData_; }

public:

	Vector4 color_;
	Vector3 direction_;
	float intensity_;

private:
	DirectionalLightData directionalLightData_;
};