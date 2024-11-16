#pragma once
#include <memory>
#include "BaseScene.h"
#include "GameScene.h"
#include "TitleScene.h"

// シーン管理
class SceneManager
{
public:

	static SceneManager* GetInstance();

	// 初期化
	void Initialize();

	void Finalize();

	// 更新　
	void Update();

	// 描画
	void Draw();

	// 次のシーンの予約
	void SetNextScene(std::unique_ptr<BaseScene> nextScene) { nextScene_ = std::move(nextScene); }


private:

	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

	// 今のシーン
	std::unique_ptr<BaseScene> scene_ = nullptr;

	// 次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;
};