#include "RailCamera.h"

void RailCamera::Initialize()
{
	viewProjecton_->Initialize();
	viewProjecton_->Update();
}

void RailCamera::Update()
{
	Vector3 eyePosition;
	Vector3 targetPosition;

	if (time < 1200)
	{
		time += 1;
	}

	// カメラ座標用の計算
	eyePosition = CatMullMove(controlPoints_, time / 1200);
	
	// カメラのターゲット座標用の計算

	// 計算結果を代入
	viewProjecton_->transform.translate = eyePosition;

	

	// 向きベクトル
	Vector3 subVector = targetPosition - eyePosition;

}
