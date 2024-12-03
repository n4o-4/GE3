#include "LightManager.h"

void Lightmanager::Initialize()
{
	lightDatasResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(LightDatas));
	
	lightDatasResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&lightDatas));

	std::string initial = "initial";

	std::unique_ptr<DirectionalLight> initialDirectionalLight = std::make_unique<DirectionalLight>();
	initialDirectionalLight->Initilaize();
	directionalLights[initial] = std::move(*initialDirectionalLight.get());

	std::unique_ptr<PointLight> initialPointLight = std::make_unique<PointLight>();
	initialPointLight->Initilize();
	pointLights[initial] = *initialPointLight.get();

	std::unique_ptr<SpotLight> initialSpotLight = std::make_unique<SpotLight>();
	initialSpotLight->Initialize();
	spotLights[initial] = *initialSpotLight;
}

void Lightmanager::Update()
{
	dLightCount_ = 0;
	pLightCount_ = 0;
	sLightCount_ = 0;

	// directionalLightの数だけ回す
	for (std::unordered_map<std::string, DirectionalLight>::iterator directionalLightIterator = directionalLights.begin(); directionalLightIterator != directionalLights.end();)
	{
		// directionalLightの更新
		directionalLightIterator->second.Update();

		lightDatas->directionalLights[dLightCount_] = directionalLightIterator->second.GetData();

		++dLightCount_;
		++directionalLightIterator;
	}

	// pointLightの数だけ回す
	for (std::unordered_map<std::string, PointLight>::iterator pointLightIterator = pointLights.begin(); pointLightIterator != pointLights.end();)
	{
		// pointLightの更新
		pointLightIterator->second.Update();

		lightDatas->pointLights[pLightCount_] = pointLightIterator->second.GetData();

		++pLightCount_;
		++pointLightIterator;
	}

	// spotLightの数だけ回す
	for (std::unordered_map<std::string, SpotLight>::iterator spotLightIterator = spotLights.begin(); spotLightIterator != spotLights.end();)
	{
		// spotLightの更新
		spotLightIterator->second.Update();

		lightDatas->spotLights[sLightCount_] = spotLightIterator->second.GetData();

		++sLightCount_;
		++spotLightIterator;
	}

	lightDatas->dLightCount = dLightCount_;
	lightDatas->pLightCount = pLightCount_;
	lightDatas->sLightCount = sLightCount_;
}
