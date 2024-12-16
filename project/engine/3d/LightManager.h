#pragma once

#include <string>
#include <unordered_map>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class LightManager
{

	struct LightDatas
	{
		int dLightCount;
		int pLightCount;
		int sLightCount;
		int padding;

		DirectionalLightData directionalLights[4];
		PointLightData pointLights[4];
		SpotLightData spotLights[4];
	};

public:

	void Initialize();
	void Update();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetCBuffer() { return lightDatasResource_; }

private:

	LightDatas* lightDatas = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> lightDatasResource_;

	std::unordered_map<std::string, DirectionalLight> directionalLights;
	std::unordered_map<std::string, PointLight> pointLights;
	std::unordered_map<std::string, SpotLight> spotLights;

	int dLightCount_ = 0;
	int pLightCount_ = 0;
	int sLightCount_ = 0;

};