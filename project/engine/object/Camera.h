#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include "MyMath.h"

class Camera
{
private:

	inline struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

public:
	Camera();

	// 更新
	void Update();

	// setter
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetFovY(float fovY) { this->fovY = fovY; }
	void SetAspectRation(float aspectRation) { this->aspectRation = aspectRation; }

	// getter
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
	const Vector3& GetRotate() const { return transform.rotate; }
	const Vector3& GetTranslate() const { return transform.translate; }	

private:
	Transform transform;
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;

	Matrix4x4 projectionMatrix;
	float fovY;
	float aspectRation;
	float nearClip;
	float farClip;

	Matrix4x4 viewProjectionMatrix;
};

