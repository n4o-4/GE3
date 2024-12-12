#pragma once
#include <cmath>

#include "Vectors.h"
#include "Matrixs.h"

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

namespace Quat
{
	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	static Quaternion Identity();

	// Quaternionのnormを返す
	static float Norm(const Quaternion& quaternion);
	
	static Quaternion Normalize(const Quaternion& quaternion);
	
	static Quaternion Inverse(const Quaternion& quaternion);
	
	static Matrix4x4 MakeRotateMatrix(const Quaternion& q);
	
	Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	// 任意回転を表すQuaternionの生成
	Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
}

static Quaternion Conjugate(const Quaternion& quaternion);

// ベクトルをQuaternionで回転させた結果のベクトルを求める
static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);