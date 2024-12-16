#pragma once
#include "BaseCamera.h"

class RailCamera : public BaseCamera
{
public:
	void Initialize() override;
	void Update() override;

private: // メンバ変数

	std::vector<Vector3> controlPoints_;

	int time = 0;
};

