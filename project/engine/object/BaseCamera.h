#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include "MyMath.h"
#include "ViewProjection.h"

class BaseCamera
{
public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;

protected:

	ViewProjection* viewProjecton_ = nullptr;
};

