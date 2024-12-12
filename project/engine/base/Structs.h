#pragma once
#include "Vectors.h"
#include "Matrixs.h"

struct Transform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

static struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};