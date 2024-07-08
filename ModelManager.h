#pragma once
#include <map>
#include <string>
#include <memory>
#include "Model.h"
#include "ModelCommon.h"

class ModelManager
{
public:
	// シングルトンインスタンスの取得
	static ModelManager* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// 終了
	void Finalize();

	// モデルファイル読み込み
	void LoadModel(const std::string& filePath);

	Model* FindModel(const std::string& filePath);

private:
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;

private:
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models;

	ModelCommon* modelCommon = nullptr;
};

