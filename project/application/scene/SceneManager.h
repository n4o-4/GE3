#pragma once
#include <memory>
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

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

	void ChangeScene(const std::string& sceneName);
	
	void SetSceneFactory(AbstaractSceneFactory& sceneFactory) { sceneFactory_ = &sceneFactory; }

	
private:

	static std::unique_ptr<SceneManager> instance;

	friend std::unique_ptr<SceneManager> std::make_unique<SceneManager>();
	friend std::default_delete<SceneManager>;

	~SceneManager() = default;
	SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

	
	// 今のシーン
	std::unique_ptr<BaseScene> scene_ = nullptr;

	// 次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	AbstaractSceneFactory* sceneFactory_ = nullptr;
};