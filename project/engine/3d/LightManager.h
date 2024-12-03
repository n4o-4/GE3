#pragma once

#include <string>
#include <unordered_map>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


class Lightmanager
{

	struct LightDatas
	{
		DirectionalLightData directionalLights[16];
		PointLightData pointLights[16];
		SpotLightData spotLights[16];

		int dLightCount;
		int pLightCount;
		int sLightCount;
		int padding[1];
	};

public:

	void Initialize();
	void Update();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetCBuffer() { return lightDatasResource_; }

private:

	std::unique_ptr<LightDatas> lightDatas = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> lightDatasResource_;

	std::unordered_map<std::string, DirectionalLight> directionalLights;
	std::unordered_map<std::string, PointLight> pointLights;
	std::unordered_map<std::string, SpotLight> spotLights;

	int dLightCount_ = 0;
	int pLightCount_ = 0;
	int sLightCount_ = 0;

};