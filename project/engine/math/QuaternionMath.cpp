#include "QuaternionMath.h"

namespace Quat
{
	Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion(
			lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
			lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
			lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
		);
	}

	Quaternion Identity()
	{
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}
	float
		Norm(const Quaternion& quaternion)
	{
		// クォータニオンのノルムを計算: √(x^2 + y^2 + z^2 + w^2)
		return sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
	}

	Quaternion Normalize(const Quaternion& quaternion)
	{
		// クォータニオンのノルムを計算
		float norm = Norm(quaternion);

		// ノルムが0でないことを確認（ゼロ除算を避ける）
		if (norm > 0.0f)
		{
			// クォータニオンを正規化
			float invNorm = 1.0f / norm;
			return Quaternion(quaternion.x * invNorm, quaternion.y * invNorm, quaternion.z * invNorm, quaternion.w * invNorm);
		}

		// ノルムがゼロの場合、ゼロのクォータニオンを返す
		return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Quaternion Inverse(const Quaternion& quaternion)
	{
		// クォータニオンのノルムを計算
		float normSquared = Norm(quaternion) * Norm(quaternion);

		// ノルムの二乗が0でないことを確認（ゼロ除算を避ける）
		if (normSquared > 0.0f)
		{
			// クォータニオンの共役を計算
			Quaternion conjugate = Conjugate(quaternion);

			// 共役をノルムの二乗で割る
			float invNormSquared = 1.0f / normSquared;
			return Quaternion(conjugate.x * invNormSquared, conjugate.y * invNormSquared, conjugate.z * invNormSquared, conjugate.w * invNormSquared);
		}

		// ノルムの二乗が0の場合、ゼロのクォータニオンを返す
		return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Matrix4x4 MakeRotateMatrix(const Quaternion& q)
	{
		Matrix4x4 matrix;

		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;
		float ww = q.w * q.w;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		matrix.m[0][0] = ww + xx - yy - zz;
		matrix.m[0][1] = 2.0f * (xy + wz);
		matrix.m[0][2] = 2.0f * (xz - wy);
		matrix.m[0][3] = 0.0f;

		matrix.m[1][0] = 2.0f * (xy - wz);
		matrix.m[1][1] = ww - xx + yy - zz;
		matrix.m[1][2] = 2.0f * (yz + wx);
		matrix.m[1][3] = 0.0f;

		matrix.m[2][0] = 2.0f * (xz + wy);
		matrix.m[2][1] = 2.0f * (yz - wx);
		matrix.m[2][2] = ww - xx - yy + zz;
		matrix.m[2][3] = 0.0f;

		matrix.m[3][0] = 0.0f;
		matrix.m[3][1] = 0.0f;
		matrix.m[3][2] = 0.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
	{
		Quaternion result;

		// クォータニオンのドット積を計算
		float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

		// ドット積が負の場合、q1 を反転して補間の最短経路を選択
		Quaternion q1Adjusted = q1;
		if (dot < 0.0f) {
			q1Adjusted.x = -q1.x;
			q1Adjusted.y = -q1.y;
			q1Adjusted.z = -q1.z;
			q1Adjusted.w = -q1.w;
			dot = -dot;
		}

		// ドット積が 1 に近い場合、線形補間を使用


		const float epsilon = 1e-6f;
		if (dot > 1.0f - epsilon) {
			result = {
				q0.x + t * (q1Adjusted.x - q0.x),
				q0.y + t * (q1Adjusted.y - q0.y),
				q0.z + t * (q1Adjusted.z - q0.z),
				q0.w + t * (q1Adjusted.w - q0.w)
			}; // 正規化して返す

			result = Quat::Normalize(result);

			return result;
		}

		// 球面線形補間を計算
		float theta = std::acos(dot); // 角度を計算
		float sinTheta = std::sqrt(1.0f - dot * dot); // sin(θ) を計算

		float w0 = std::sin((1.0f - t) * theta) / sinTheta;
		float w1 = std::sin(t * theta) / sinTheta;

		result = {
			w0 * q0.x + w1 * q1Adjusted.x,
			w0 * q0.y + w1 * q1Adjusted.y,
			w0 * q0.z + w1 * q1Adjusted.z,
			w0 * q0.w + w1 * q1Adjusted.w
		};

		return result;
	}

	Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
	{
		float halfAngle = angle * 0.5f;
		float sinHalfAngle = std::sin(halfAngle);

		return Quaternion(
			axis.x * sinHalfAngle,  // x component
			axis.y * sinHalfAngle,  // y component
			axis.z * sinHalfAngle,  // z component
			std::cos(halfAngle)     // w component
		);
	}
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	// クォータニオンの共役を計算
	return Quaternion(-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w);
}



Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion q = quaternion;

	Quaternion vectorQuat(vector.x, vector.y, vector.z, 0.0f);
	Quaternion conjugateQuat = Conjugate(q);

	Quaternion rotatedQuat = Quat::Multiply(Quat::Multiply(quaternion, vectorQuat), conjugateQuat);

	return Vector3(rotatedQuat.x, rotatedQuat.y, rotatedQuat.z);
}