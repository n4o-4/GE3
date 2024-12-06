#pragma once

#include <memory>

class SceneManager;

// シーン基底クラス
class BaseScene
{
public:

	virtual ~BaseScene() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:
	SceneManager* sceneManager_ = nullptr;

};