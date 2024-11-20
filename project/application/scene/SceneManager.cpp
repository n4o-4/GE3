#include "SceneManager.h"
#include <assert.h>

std::unique_ptr<SceneManager> SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance =  std::make_unique<SceneManager>();
	}

	return instance.get();
}

void SceneManager::Initialize()
{
}

void SceneManager::Finalize()
{
	instance.reset();
}

void SceneManager::Update()
{

	if (nextScene_)
	{
		// 
		if (scene_)
		{
			scene_.reset();
			scene_ = nullptr;
		}

		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;
		
		scene_->Initialize();

		scene_->SetSceneManager(this);
	}

	// 実行中のシーンの更新
	scene_->Update();
}

void SceneManager::Draw()
{

	// 実行中のシーンの描画
	scene_->Draw();

}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーン生成
	nextScene_ = std::move(sceneFactory_->CreateScene(sceneName));
}
