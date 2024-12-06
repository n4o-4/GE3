﻿#pragma once

#include "Vectors.h"
#include "Matrixs.h"
#include <math.h>
#include <numbers>

const float kDeltaTime = 1.0f / 60.0f;

struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

inline Vector3 Normalize(Vector3 v)
{
	Vector3 RVector3;
	float lenght = sqrtf(v.x * v.x + v.y * v.y);
	lenght = sqrtf(lenght * lenght + v.z * v.z);
	RVector3 = { (v.x / lenght),(v.y / lenght),(v.z / lenght) };
	return RVector3;
}

static Matrix4x4 MakeRotateXMatrix(float rotate)
{
	Matrix4x4 rM{};
	rM.m[0][0] = 1;
	rM.m[1][1] = cosf(rotate);
	rM.m[1][2] = sinf(rotate);
	rM.m[2][1] = -sinf(rotate);
	rM.m[2][2] = cosf(rotate);
	rM.m[3][3] = 1;

	return rM;
}

static Matrix4x4 MakeRotateYMatrix(float rotate)
{
	Matrix4x4 rM{};
	rM.m[0][0] = cosf(rotate);
	rM.m[0][2] = -sinf(rotate);
	rM.m[1][1] = 1;
	rM.m[2][0] = sinf(rotate);
	rM.m[2][2] = cosf(rotate);
	rM.m[3][3] = 1;

	return rM;
}

static Matrix4x4 MakeRotateZMatrix(float rotate)
{
	Matrix4x4 rM{};
	rM.m[0][0] = cosf(rotate);
	rM.m[0][1] = sinf(rotate);
	rM.m[1][0] = -sinf(rotate);
	rM.m[1][1] = cosf(rotate);
	rM.m[2][2] = 1;
	rM.m[3][3] = 1;

	return rM;
}

static Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2)
{
	Matrix4x4 resultMatrix{};
	float num = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				num = num + m1.m[i][k] * m2.m[k][j];
			}
			resultMatrix.m[i][j] = num;
			num = 0;
		}
	}
	return resultMatrix;

	
}

static Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 Translate)
{
	Matrix4x4 sM{};
	Matrix4x4 tM{};
	Matrix4x4 resultMatrix{};

	//scaleMatrix
	sM.m[0][0] = scale.x;
	sM.m[0][1] = 0;
	sM.m[0][2] = 0;
	sM.m[1][0] = 0;
	sM.m[1][1] = scale.y;
	sM.m[1][2] = 0;
	sM.m[2][0] = 0;
	sM.m[2][1] = 0;
	sM.m[2][2] = scale.z;
	sM.m[3][3] = 1;

	//rotateMatrix
	Matrix4x4 rXM = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rYM = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rZM = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rXYZM =Multiply(rXM, Multiply(rYM, rZM));
	
	//translateMatrix
	tM.m[0][0] = 1;
	tM.m[1][1] = 1;
	tM.m[2][2] = 1;
	tM.m[3][0] = Translate.x;
	tM.m[3][1] = Translate.y;
	tM.m[3][2] = Translate.z;
	tM.m[3][3] = 1;

	resultMatrix.m[0][0] = sM.m[0][0] * rXYZM.m[0][0];
	resultMatrix.m[0][1] = sM.m[0][0] * rXYZM.m[0][1];
	resultMatrix.m[0][2] = sM.m[0][0] * rXYZM.m[0][2];

	resultMatrix.m[1][0] = sM.m[1][1] * rXYZM.m[1][0];
	resultMatrix.m[1][1] = sM.m[1][1] * rXYZM.m[1][1];
	resultMatrix.m[1][2] = sM.m[1][1] * rXYZM.m[1][2];

	resultMatrix.m[2][0] = sM.m[2][2] * rXYZM.m[2][0];
	resultMatrix.m[2][1] = sM.m[2][2] * rXYZM.m[2][1];
	resultMatrix.m[2][2] = sM.m[2][2] * rXYZM.m[2][2];


	resultMatrix.m[3][0] = tM.m[3][0];
	resultMatrix.m[3][1] = tM.m[3][1];
	resultMatrix.m[3][2] = tM.m[3][2];
	resultMatrix.m[3][3] = 1;

	return resultMatrix;
}

static Matrix4x4 MakeIdentity4x4()
{
	Matrix4x4 resultMatrix{};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				resultMatrix.m[i][j] = 1;
			}
			else
			{
				resultMatrix.m[i][j] = 0;
			}
		}
	}
	return resultMatrix;
}

static Matrix4x4 Inverse(Matrix4x4 matrix)
{
	Matrix4x4 resultMatrix{};
	float A =
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]

		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]//

		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]

		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]

		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];


	resultMatrix.m[0][0] =
		(matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
			matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
			matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
			matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[0][1] =
		(-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] -
			matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] -
			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] +
			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] +
			matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] +
			matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[0][2] =
		(matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] +
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] +
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] -
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] -
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]) / A;

	resultMatrix.m[0][3] =
		(-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] -
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] -
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] +
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] +
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] +
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]) / A;

	resultMatrix.m[1][0] =
		(-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
			matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
			matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] +
			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
			matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
			matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[1][1] =
		(matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] +
			matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] +
			matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] -
			matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] -
			matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[1][2] =

		(-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] -
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] -
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] +
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] +
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] +
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]) / A;

	resultMatrix.m[1][3] =
		(matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] -
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) / A;

	resultMatrix.m[2][0] =
		(matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
			matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
			matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
			matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
			matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
			matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) / A;

	resultMatrix.m[2][1] =
		(-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] -
			matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] -
			matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] +
			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] +
			matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] +
			matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]) / A;

	resultMatrix.m[2][2] =
		(matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] +
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] +
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] -
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] -
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]) / A;

	resultMatrix.m[2][3] =
		(-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] -
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] -
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] +
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] +
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] +
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]) / A;

	resultMatrix.m[3][0] =
		(-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] -
			matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] -
			matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] +
			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
			matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] +
			matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) / A;

	resultMatrix.m[3][1] =
		(matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] +
			matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] +
			matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
			matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] -
			matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] -
			matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]) / A;

	resultMatrix.m[3][2] =
		(-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] -
			matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] -
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] +
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] +
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] +
			matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]) / A;

	resultMatrix.m[3][3] =
		(matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
			matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
			matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]) / A;

	return resultMatrix;
}

static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRation, float nearClip, float farClip)
{
	float tanHalfFovY = tanf(fovY * 0.5f);
	float scaleX = 1.0f / (aspectRation * tanHalfFovY);
	float scaleY = 1.0f / tanHalfFovY;
	Matrix4x4 resultMatrix{};
	resultMatrix.m[0][0] = scaleX;
	resultMatrix.m[1][1] = scaleY;
	resultMatrix.m[2][2] = farClip / (farClip - nearClip);
	resultMatrix.m[2][3] = 1;
	resultMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return resultMatrix;
}

static Matrix4x4 MakeOrthographicMatrix(float l, float t, float r, float b, float zn, float zf)
{
	Matrix4x4 resultMatrix = {};
	resultMatrix.m[0][0] = 2 / (r - l);
	resultMatrix.m[1][1] = 2 / (t - b);
	resultMatrix.m[2][2] = -2 / (zf - zn);
	resultMatrix.m[3][0] = -(r + l) / (r - l);
	resultMatrix.m[3][1] = -(t + b) / (t - b);
	resultMatrix.m[3][2] = -(zf + zn) / (zf - zn);
	resultMatrix.m[3][3] = 1;

	return resultMatrix;
}

inline bool IsCollision(const AABB& aabb, const Vector3& point)
{
	if ((aabb.min.x <= point.x && aabb.max.x >= point.x) &&
		(aabb.min.y <= point.y && aabb.max.y >= point.y) &&
		(aabb.min.z <= point.z && aabb.max.z >= point.z)) {

		return true;
	}

	return false;
}