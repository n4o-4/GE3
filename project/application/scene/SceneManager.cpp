#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager();
	}

	return instance;
}

void SceneManager::Initialize()
{

	scene_ = std::make_unique<TitleScene>();

	scene_->Initialize();

	scene_->SetSceneManager(this);
}

void SceneManager::Finalize()
{
	delete instance;
	instance = nullptr;
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
