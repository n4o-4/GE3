#include "Camera.h"
#include "WinApp.h"

Camera::Camera()
{
	/*transform = { { 1.0f,1.0f,1.0f },{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
	fovY = 0.45f;
	aspectRation = static_cast<float>(WinApp::kClientWidth) / static_cast<float>(WinApp::kClientHeight);
	nearClip = 0.1f;
	farClip = 100.0f;
	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	viewMatrix = Inverse(worldMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRation, nearClip, farClip);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);*/

	viewProjection.Initialize();
}

void Camera::Update()
{
	worldMatrix = MakeAffineMatrix(viewProjection.transform.scale, viewProjection.transform.rotate, viewProjection.transform.translate);

	viewProjection.Update();
}
