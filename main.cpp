#include <string>
//#include <format>
#include <cassert>
#include <dxgidebug.h>
#include <debugapi.h>

#include "externals/DirectXTex/DirectXTex.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <sstream>
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"



//class ResourceObject {
//public:
//	ResourceObject(ID3D12Resource* resource)
//		:resource_(resource)
//	{}
//
//	~ResourceObject() {
//		if (resource_) {
//			resource_->Release();
//		}
//	}
//
//	ID3D12Resource* Get() { return resource_; }
//private:
//	ID3D12Resource* resource_;
//};

//struct Vector2
//{
//	float v;
//	float u;
//};
//
//struct Vector3
//{
//	float x;
//	float y;
//	float z;
//};
//
//struct Vector4
//{
//	float x;
//	float y;
//	float z;
//	float w;
//};
//
//struct Matrix3x3
//{
//	float m[3][3];
//};
//
//struct VertexData
//{
//	Vector4 position;
//	Vector2 texcoord;
//	Vector3 normal;
//};
//
//struct Matrix4x4
//{
//	float m[4][4];
//};
//
//struct Material {
//	Vector4 color;
//	int enableLighting;
//	float padding[3];
//	Matrix4x4 uvTransform;
//};
//
//struct TransformationMatrix {
//	Matrix4x4 WVP;
//	Matrix4x4 World;
//};
//
//struct Transform
//{
//	Vector3 scale;
//	Vector3 rotate;
//	Vector3 translate;
//};
//
//struct DirectionLight {
//	Vector4 color;
//	Vector3 direction;
//	float intensity;
//};
//
//struct MaterialData
//{
//	std::string textureFilePath;
//};
//
//struct ModelData {
//	std::vector<VertexData> vertices;
//	MaterialData material;
//};
//
//Vector3 Normalize(Vector3 v)
//{
//	Vector3 RVector3;
//	float lenght = sqrtf(v.x * v.x + v.y * v.y);
//	lenght = sqrtf(lenght * lenght + v.z * v.z);
//	RVector3 = { (v.x / lenght),(v.y / lenght),(v.z / lenght) };
//	return RVector3;
//}
//
//Matrix4x4 MakeRotateXMatrix(float rotate)
//{
//	Matrix4x4 rM{};
//	rM.m[0][0] = 1;
//	rM.m[1][1] = cosf(rotate);
//	rM.m[1][2] = sinf(rotate);
//	rM.m[2][1] = -sinf(rotate);
//	rM.m[2][2] = cosf(rotate);
//	rM.m[3][3] = 1;
//
//	return rM;
//}
//
//Matrix4x4 MakeRotateYMatrix(float rotate)
//{
//	Matrix4x4 rM{};
//	rM.m[0][0] = cosf(rotate);
//	rM.m[0][2] = -sinf(rotate);
//	rM.m[1][1] = 1;
//	rM.m[2][0] = sinf(rotate);
//	rM.m[2][2] = cosf(rotate);
//	rM.m[3][3] = 1;
//
//	return rM;
//}
//
//Matrix4x4 MakeRotateZMatrix(float rotate)
//{
//	Matrix4x4 rM{};
//	rM.m[0][0] = cosf(rotate);
//	rM.m[0][1] = sinf(rotate);
//	rM.m[1][0] = -sinf(rotate);
//	rM.m[1][1] = cosf(rotate);
//	rM.m[2][2] = 1;
//	rM.m[3][3] = 1;
//
//	return rM;
//}
//
//Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2)
//{
//	Matrix4x4 resultMatrix;
//	float num = 0;
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			for (int k = 0; k < 4; k++)
//			{
//				num = num + m1.m[i][k] * m2.m[k][j];
//			}
//			resultMatrix.m[i][j] = num;
//			num = 0;
//		}
//	}
//	return resultMatrix;
//
//	
//}
//
//Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 Translate)
//{
//	Matrix4x4 sM{};
//	Matrix4x4 tM{};
//	Matrix4x4 resultMatrix{};
//
//	//scaleMatrix
//	sM.m[0][0] = scale.x;
//	sM.m[0][1] = 0;
//	sM.m[0][2] = 0;
//	sM.m[1][0] = 0;
//	sM.m[1][1] = scale.y;
//	sM.m[1][2] = 0;
//	sM.m[2][0] = 0;
//	sM.m[2][1] = 0;
//	sM.m[2][2] = scale.z;
//	sM.m[3][3] = 1;
//
//	//rotateMatrix
//	Matrix4x4 rXM = MakeRotateXMatrix(rotate.x);
//	Matrix4x4 rYM = MakeRotateYMatrix(rotate.y);
//	Matrix4x4 rZM = MakeRotateZMatrix(rotate.z);
//	Matrix4x4 rXYZM =Multiply(rXM, Multiply(rYM, rZM));
//	
//	//translateMatrix
//	tM.m[0][0] = 1;
//	tM.m[1][1] = 1;
//	tM.m[2][2] = 1;
//	tM.m[3][0] = Translate.x;
//	tM.m[3][1] = Translate.y;
//	tM.m[3][2] = Translate.z;
//	tM.m[3][3] = 1;
//
//	resultMatrix.m[0][0] = sM.m[0][0] * rXYZM.m[0][0];
//	resultMatrix.m[0][1] = sM.m[0][0] * rXYZM.m[0][1];
//	resultMatrix.m[0][2] = sM.m[0][0] * rXYZM.m[0][2];
//
//	resultMatrix.m[1][0] = sM.m[1][1] * rXYZM.m[1][0];
//	resultMatrix.m[1][1] = sM.m[1][1] * rXYZM.m[1][1];
//	resultMatrix.m[1][2] = sM.m[1][1] * rXYZM.m[1][2];
//
//	resultMatrix.m[2][0] = sM.m[2][2] * rXYZM.m[2][0];
//	resultMatrix.m[2][1] = sM.m[2][2] * rXYZM.m[2][1];
//	resultMatrix.m[2][2] = sM.m[2][2] * rXYZM.m[2][2];
//
//
//	resultMatrix.m[3][0] = tM.m[3][0];
//	resultMatrix.m[3][1] = tM.m[3][1];
//	resultMatrix.m[3][2] = tM.m[3][2];
//	resultMatrix.m[3][3] = 1;
//
//	return resultMatrix;
//}
//
//
//
//
//Matrix4x4 MakeIdentity4x4()
//{
//	Matrix4x4 resultMatrix;
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			if (i == j)
//			{
//				resultMatrix.m[i][j] = 1;
//			}
//			else
//			{
//				resultMatrix.m[i][j] = 0;
//			}
//		}
//	}
//	return resultMatrix;
//}
//
//Matrix4x4 Inverse(const Matrix4x4& matrix)
//{
//	Matrix4x4 resultMatrix;
//	float A =
//		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]
//
//		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]
//
//		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]//
//
//		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]
//
//		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]
//
//		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]
//
//		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]
//
//		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];
//
//
//	resultMatrix.m[0][0] =
//		(matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
//			matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
//			matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
//			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
//			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
//			matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) / A;
//
//	resultMatrix.m[0][1] =
//		(-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] -
//			matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] -
//			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] +
//			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] +
//			matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] +
//			matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]) / A;
//
//	resultMatrix.m[0][2] =
//		(matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] +
//			matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] +
//			matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
//			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] -
//			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] -
//			matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]) / A;
//
//	resultMatrix.m[0][3] =
//		(-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] -
//			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] -
//			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] +
//			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] +
//			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] +
//			matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]) / A;
//
//	resultMatrix.m[1][0] =
//		(-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
//			matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
//			matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] +
//			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
//			matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
//			matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) / A;
//
//	resultMatrix.m[1][1] =
//		(matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] +
//			matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] +
//			matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
//			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] -
//			matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] -
//			matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]) / A;
//
//	resultMatrix.m[1][2] =
//
//		(-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] -
//			matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] -
//			matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] +
//			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] +
//			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] +
//			matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]) / A;
//
//	resultMatrix.m[1][3] =
//		(matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
//			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
//			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
//			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
//			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] -
//			matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) / A;
//
//	resultMatrix.m[2][0] =
//		(matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
//			matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
//			matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
//			matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
//			matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
//			matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) / A;
//
//	resultMatrix.m[2][1] =
//		(-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] -
//			matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] -
//			matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] +
//			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] +
//			matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] +
//			matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]) / A;
//
//	resultMatrix.m[2][2] =
//		(matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] +
//			matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] +
//			matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
//			matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] -
//			matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] -
//			matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]) / A;
//
//	resultMatrix.m[2][3] =
//		(-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] -
//			matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] -
//			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] +
//			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] +
//			matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] +
//			matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]) / A;
//
//	resultMatrix.m[3][0] =
//		(-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] -
//			matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] -
//			matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] +
//			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
//			matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] +
//			matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) / A;
//
//	resultMatrix.m[3][1] =
//		(matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] +
//			matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] +
//			matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
//			matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] -
//			matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] -
//			matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]) / A;
//
//	resultMatrix.m[3][2] =
//		(-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] -
//			matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] -
//			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] +
//			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] +
//			matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] +
//			matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]) / A;
//
//	resultMatrix.m[3][3] =
//		(matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
//			matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
//			matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
//			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
//			matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
//			matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]) / A;
//
//	return resultMatrix;
//}
//
//Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRation, float nearClip, float farClip)
//{
//	float tanHalfFovY = tanf(fovY * 0.5f);
//	float scaleX = 1.0f / (aspectRation * tanHalfFovY);
//	float scaleY = 1.0f / tanHalfFovY;
//	Matrix4x4 resultMatrix{};
//	resultMatrix.m[0][0] = scaleX;
//	resultMatrix.m[1][1] = scaleY;
//	resultMatrix.m[2][2] = farClip / (farClip - nearClip);
//	resultMatrix.m[2][3] = 1;
//	resultMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
//
//	return resultMatrix;
//}
//
//Matrix4x4 MakeOrthographicMatrix(float l, float t, float r, float b, float zn, float zf)
//{
//	/*Matrix4x4 resultMatrix = {};
//	resultMatrix.m[0][0] = 2 / (r - l);
//	resultMatrix.m[1][1] = 2 / (t - b);
//	resultMatrix.m[2][2] = 1 / (zf - zn);
//	resultMatrix.m[3][0] = (l + r) / (l - r);
//	resultMatrix.m[3][1] = (t + b) / (b - t);
//	resultMatrix.m[3][2] = zn / (zn - zf);
//	resultMatrix.m[3][3] = 1;
//
//	return resultMatrix;*/
//
//	Matrix4x4 resultMatrix = {};
//	resultMatrix.m[0][0] = 2 / (r - l);
//	resultMatrix.m[1][1] = 2 / (t - b);
//	resultMatrix.m[2][2] = -2 / (zf - zn);
//	resultMatrix.m[3][0] = -(r + l) / (r - l);
//	resultMatrix.m[3][1] = -(t + b) / (t - b);
//	resultMatrix.m[3][2] = -(zf + zn) / (zf - zn);
//	resultMatrix.m[3][3] = 1;
//
//	return resultMatrix;
//}
//
//IDxcBlob* CompileShader(
//	// CompilerするShaderファイルへのパス
//	const std::wstring& filePath,
//
//	// Compilerに使用するProfile
//	const wchar_t* profile,
//
//	// 初期化で生成したもの３つ
//	IDxcUtils* dxcUtils,
//	IDxcCompiler3* dxcCompiler,
//	IDxcIncludeHandler* includeHandler)
//{
//	// これからシェーダーをコンパイル
//	//Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
//
//	// hlslファイルを読み込む
//	IDxcBlobEncoding* shaderSource = nullptr;
//	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
//
//	// 読めなかったら止める
//	assert(SUCCEEDED(hr));
//
//	// 読み込んだファイルの内容を設定する
//	DxcBuffer shaderSourceBuffer;
//	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
//	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
//	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTFの文字コードであることを通知
//
//	LPCWSTR arguments[] =
//	{
//		filePath.c_str(), // コンパイル対象のhlslファイル名
//		L"-E",L"main", // エントリーポイントの指定。基本的にmain以外にはしない
//		L"-T", profile, // ShaderProfileの設定
//		L"-Zi", L"-Qembed_debug", // デバッグ用の情報を埋め込む
//		L"-Od",    // 最適化しておく
//		L"-Zpr",   // メモリレイアウトは最優先
//	};
//
//	// 実際にShaderをコンパイルする
//	IDxcResult* shaderResult = nullptr;
//	hr = dxcCompiler->Compile(
//		&shaderSourceBuffer,
//		arguments,
//		_countof(arguments),
//		includeHandler,
//		IID_PPV_ARGS(&shaderResult)
//	);
//
//	// コンパイルエラーでなくdxcが起動出来ないなど致命的な状況
//	assert(SUCCEEDED(hr));
//
//	// 警告・エラーが出たらログを出して止める
//	IDxcBlobUtf8* shaderError = nullptr;
//	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
//	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
//	{
//		//Log(shaderError->GetStringPointer());
//
//		// 警告・エラー
//		assert(false);
//	}
//
//	// コンパイル結果から実行のバイナリ部分を取得
//	IDxcBlob* shaderBlob = nullptr;
//	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
//	assert(SUCCEEDED(hr));
//
//	// 成功したログを出す
//	//Log(ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile)));
//
//	// もう使わないリソースを解放
//	shaderSource->Release();
//	shaderResult->Release();
//
//	// 実行用のバイナリを返却
//	return shaderBlob;
//
//}


//Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, size_t sizeInBytes)
//{
//	//ID3D12Resource* resultResource;
//	Microsoft::WRL::ComPtr<ID3D12Resource> resultResource;
//	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
//	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
//	D3D12_RESOURCE_DESC vertexBufferDesc{};
//	vertexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	vertexBufferDesc.Width = sizeInBytes;
//	vertexBufferDesc.Height = 1;
//	vertexBufferDesc.DepthOrArraySize = 1;
//	vertexBufferDesc.MipLevels = 1;
//	vertexBufferDesc.SampleDesc.Count = 1;
//	vertexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resultResource));
//	return resultResource;
//}

//// Textureデータを読み込む関数
//DirectX::ScratchImage LoadTexture(const std::string& filePath)
//{
//	// テクスチャファイルを読んでプログラムで扱えるようにする
//	DirectX::ScratchImage image{};
//	std::wstring filePathW = ConvertString(filePath);
//	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
//	assert(SUCCEEDED(hr));
//
//	// ミニマップの生成
//	DirectX::ScratchImage mipImages{};
//	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
//	assert(SUCCEEDED(hr));
//
//	// ミニマップ付きデータを返す
//	return mipImages;
//}

//// 読み込んだTexture情報をもとにTextureResourceを作る関数
//Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device,const DirectX::TexMetadata& metadata)
//{
//	// metadataを基にResourceの設定
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc.Width = UINT(metadata.width); // Textureの幅
//	resourceDesc.Height = UINT(metadata.height); // Textureの高さ
//	resourceDesc.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
//	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行 or 配列Textureの配列数
//	resourceDesc.Format = metadata.format; // TextureのFormat
//	resourceDesc.SampleDesc.Count = 1; // サンプルカウント。1固定
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // Textureの次元数
//
//	// 利用するHeapの設定。
//	D3D12_HEAP_PROPERTIES heapProperties{};
//	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; // 細かい設定を行う
//	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WRITEBACKポリシーでCPUにアクセス
//	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置
//
//	// Rwsourceの生成
//	//ID3D12Resource* resource = nullptr;
//	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
//	HRESULT hr = device->CreateCommittedResource(
//		&heapProperties,
//		D3D12_HEAP_FLAG_NONE,
//		&resourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&resource));
//	assert(SUCCEEDED(hr));
//	return resource;
//}

//// データを転送するUploadTextureData関数
//void UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages)
//{
//	// meta情報を取得
//	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
//
//	// MipMapについて
//	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel)
//	{
//		// mipMapLevelを指定して各Imageを取得
//		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
//
//		// Textureに転送
//		HRESULT hr = texture->WriteToSubresource(
//			UINT(mipLevel),
//			nullptr,              // 全領域へコピー
//			img->pixels,          // 元データアドレス
//			UINT(img->rowPitch),  // 1ラインサイズ
//			UINT(img->slicePitch) // 1枚サイズ
//		);
//		assert(SUCCEEDED(hr));
//	}
//
//}


//MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
//{
//	MaterialData materialData;
//	std::string line;
//	std::ifstream file(directoryPath + "/" + filename);
//	assert(file.is_open());
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier;
//
//		// identifierに応じた処理
//		if (identifier == "map_Kd") {
//			std::string textureFilename;
//			s >> textureFilename;
//
//			// 連結してファイルパスにする
//			materialData.textureFilePath = directoryPath + "/" + textureFilename;
//		}
//	}
//	return materialData;
//}

//ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename)
//{
//	ModelData modelData;
//	std::vector<Vector4> positions; // 位置
//	std::vector<Vector3> normals; // 法線
//	std::vector<Vector2> texcoords; // テクスチャ座標
//	std::string line; // ファイルから読んだ1行を格納するもの
//
//	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
//	assert(file.is_open()); // 開けなかったら止める
//
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier; // 先頭の識別子を読む
//
//		// identifierに応じた処理
//
//		if (identifier == "v") {
//			Vector4 position;
//			s >> position.x >> position.y >> position.z;
//			position.x *= -1.0f;
//			position.w = 1.0f;
//			positions.push_back(position);
//		}
//		else if (identifier == "vt") {
//			Vector2 texcoord;
//			s >> texcoord.v >> texcoord.u;
//			texcoord.u = 1.0f - texcoord.u;
//			texcoords.push_back(texcoord);
//		}
//		else if (identifier == "vn") {
//			Vector3 normal;
//			s >> normal.x >> normal.y >> normal.z;
//			normal.x *= -1.0f;
//			normals.push_back(normal);
//		}
//		else if (identifier == "f") {
//			VertexData triangle[3];
//
//			// 面は三角形限定。その他は未対応
//			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
//				std::string vertexDefinition;
//				s >> vertexDefinition;
//
//				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
//				std::istringstream v(vertexDefinition);
//				uint32_t elementIndices[3];
//				for (int32_t element = 0; element < 3; ++element) {
//					std::string index;
//					std::getline(v, index, '/'); // /区切りでインデックスを読んでいく
//					elementIndices[element] = std::stoi(index);
//				}
//
//				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
//				Vector4 position = positions[elementIndices[0] - 1];
//				Vector2 texcoord = texcoords[elementIndices[1] - 1];
//				Vector3 normal = normals[elementIndices[2] - 1];
//				/*VertexData vertex = { position,texcoord,normal };
//				modelData.vertices.push_back(vertex);*/
//				triangle[faceVertex] = { position, texcoord,normal };
//			}
//
//			modelData.vertices.push_back(triangle[2]);
//			modelData.vertices.push_back(triangle[1]);
//			modelData.vertices.push_back(triangle[0]);
//		}
//		else if (identifier == "mtllib") {
//			// materialTemplateLibraryファイルの名前を取得する
//			std::string materialFilename;
//			s >> materialFilename;
//
//			// 基本的にObjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
//			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
//		}
//	}
//	return modelData;
//}

//struct D3D12ResourceLeakChecker {
//	~D3D12ResourceLeakChecker()
//	{
//		//
//		Microsoft::WRL::ComPtr<IDXGIDebug> debug;
//		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
//		{
//			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
//			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
//			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
//		}
//	}
//};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//D3D12ResourceLeakChecker leakChecker;

	WinApp* winApp = nullptr;

	winApp = new WinApp();
	winApp->Initialize();

	// ポインタ
	Input* input = nullptr;

	// 入力の初期化
	input = new Input();
	input->Initialize(winApp);

	DirectXCommon* directXC = nullptr;

	directXC = new DirectXCommon();
	directXC->Initialize(winApp);

	/*input->Update();
	delete input;*/



	//// RootSignature作成
	//D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	//descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//descriptorRange[0].BaseShaderRegister = 0; // 0からはじまる
	//descriptorRange[0].NumDescriptors = 1; // 1から始まる
	//descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	//descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	//// RootParameter作成
	//D3D12_ROOT_PARAMETER rootParameters[4] = {};
	//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  //PixelShaderで使う
	//rootParameters[0].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド
	//rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//rootParameters[1].Descriptor.ShaderRegister = 0;

	//rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	//rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで行う
	//rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	//rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数


	//rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParameters[3].Descriptor.ShaderRegister = 1;


	//descriptionRootSignature.pParameters = rootParameters;  // ルートパラメーター配列へのポインタ
	//descriptionRootSignature.NumParameters = _countof(rootParameters);  // 配列の長さ

	//D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイナリフィルタ
	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	//staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;     // 多くのMipMapを使う
	//staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	//staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//descriptionRootSignature.pStaticSamplers = staticSamplers;
	//descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	////シリアライズしてバイナリにする
	////ID3DBlob* signatureBlob = nullptr;
	//Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
	////ID3DBlob* errorBlob = nullptr;
	//Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;;
	//hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	//if (FAILED(hr))
	//{
	//	Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	//	assert(false);
	//}

	//// バイナリを元に生成
	////ID3D12RootSignature* rootSignature = nullptr;
	//Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;
	//hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	//assert(SUCCEEDED(hr));


	//// モデル読み込み
	//ModelData modelData = LoadObjFile("Resources", "axis.obj");


	//// Textureを読んで転送する
	//DirectX::ScratchImage mipImages = LoadTexture("Resources/uvChecker.png");
	//const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	////ID3D12Resource* textureResource = CreateTextureResource(device, metadata);
	////ResourceObject  textureResource = CreateTextureResource(device, metadata);
	//Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = CreateTextureResource(device.Get(), metadata);
	//UploadTextureData(textureResource.Get(), mipImages);

	//// 2枚目のTextureを読んで転送する
	//DirectX::ScratchImage mipImages2 = LoadTexture(modelData.material.textureFilePath);
	//const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	////ID3D12Resource* textureResource2 = CreateTextureResource(device, metadata2);
	////ResourceObject textureResource2 = CreateTextureResource(device, metadata2);
	//Microsoft::WRL::ComPtr<ID3D12Resource> textureResource2 = CreateTextureResource(device.Get(), metadata2);
	//UploadTextureData(textureResource2.Get(), mipImages2);

	//// InputLayout
	//D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	//inputElementDescs[0].SemanticName = "POSITION";
	//inputElementDescs[0].SemanticIndex = 0;
	//inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//inputElementDescs[1].SemanticName = "TEXCOORD";
	//inputElementDescs[1].SemanticIndex = 0;
	//inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	//inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//inputElementDescs[2].SemanticName = "NORMAL";
	//inputElementDescs[2].SemanticIndex = 0;
	//inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	//inputLayoutDesc.pInputElementDescs = inputElementDescs;
	//inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//// BlendStateの設定
	//D3D12_BLEND_DESC blendDesc{};

	//// すべての要素数を書き込む
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//// RasiterzerStateの設定
	//D3D12_RASTERIZER_DESC rasterizerDesc{};

	//// 裏面(時計回り)を表示しない
	//rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	//// 三角形の中を塗りつぶす
	//rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//// Shaderをコンパイルする
	////IDxcBlob* vertexShaderBlob = CompileShader(L"Object3d.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	//Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = CompileShader(L"Resources/shaders/Object3d.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	//assert(vertexShaderBlob != nullptr);

	////IDxcBlob* pixelShaderBlob = CompileShader(L"Object3d.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	//Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = CompileShader(L"Resources/shaders/Object3d.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	//assert(pixelShaderBlob != nullptr);

	//// DepthStencilStateの設定
	//D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	//// Depthの機能を有効化する
	//depthStencilDesc.DepthEnable = true;

	//// 書き込みします
	//depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

	//// 比較関数はLessEqual。近ければ描画される
	//depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//graphicsPipelineStateDesc.pRootSignature = rootSignature.Get(); // RootSignature
	//graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;  // InputLayout
	//graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() }; // VertexShader
	//graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };   // PixelShader
	//graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
	//graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState

	//// 書き込むRTVの情報
	//graphicsPipelineStateDesc.NumRenderTargets = 1;
	//graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//// 利用するトポロジ(形状)のタイプ.。三角形
	//graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//// どのように画面に色をつけるか
	//graphicsPipelineStateDesc.SampleDesc.Count = 1;
	//graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//// DepthStencilの設定
	//graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	//graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	/////

	//// 実際に生成
	////ID3D12PipelineState* graphicsPipelineState = nullptr;
	//Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
	//hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	//assert(SUCCEEDED(hr));

	////===================
	//// directionlight
	////===================
	////ID3D12Resource* directionLightResource = CreateBufferResource(device, sizeof(DirectionLight));
	////ResourceObject directionLightResource = CreateBufferResource(device, sizeof(DirectionLight));
	//Microsoft::WRL::ComPtr<ID3D12Resource> directionLightResource = CreateBufferResource(device.Get(), sizeof(DirectionLight));
	//DirectionLight* directionLightData = nullptr;
	//directionLightResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionLightData));
	//directionLightData->color = {1.0f,1.0f,1.0f,1.0f };
	//directionLightData->direction = { 1.0f,0.0f,0.0f };
	//directionLightData->intensity = 1.0f;

	//// マテリアル用のリソースを作る
	////ID3D12Resource* materialResource = CreateBufferResource(device, sizeof(Material));
	////ResourceObject materialResource = CreateBufferResource(device, sizeof(Material));
	//Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(device.Get(), sizeof(Material));

	//// マテリアルにデータを書き込む
	//Material* materialData = nullptr;

	//// 書き込むためのアドレスを取得
	//materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//// 三角色
	//materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f, };
	//materialData->enableLighting = true;

	////WVP用のリソースを作る。Matrix4x4 １つ分のサイズを用意する
	////ID3D12Resource* wvpResource = CreateBufferResource(device, sizeof(TransformationMatrix));
	////ResourceObject wvpResource = CreateBufferResource(device, sizeof(TransformationMatrix));
	//Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = CreateBufferResource(device.Get(), sizeof(TransformationMatrix));

	//// データを書き込む
	//TransformationMatrix* wvpData = nullptr;

	//// データを書き込むためのアドレスを取得
	//wvpResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	////　単位行列を書き込んでおく
	//wvpData->WVP = MakeIdentity4x4();
	//wvpData->World = MakeIdentity4x4();

	//// 頂点リソース用のヒープの設定
	//D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	//uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // uploadHeapを使う

	//// 頂点リソースの設定
	//D3D12_RESOURCE_DESC vertexResourceDesc{};

	//// バッファリソース。テクスチャの場合また別の設定をする
	//vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//vertexResourceDesc.Width = sizeof(VertexData) * modelData.vertices.size();

	//// バッファの場合はこれらは１にする決まり
	//vertexResourceDesc.Height = 1;
	//vertexResourceDesc.DepthOrArraySize = 1;
	//vertexResourceDesc.MipLevels = 1;
	//vertexResourceDesc.SampleDesc.Count = 1;

	//// バッファの場合はこれにする決まり
	//vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//

	//// 実際に頂点リソースを作る
	////ID3D12Resource* vertexResource = CreateBufferResource(device, sizeof(VertexData) * modelData.vertices.size());
	////ResourceObject vertexResource = CreateBufferResource(device, sizeof(VertexData) * modelData.vertices.size());
	//Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device.Get(), sizeof(VertexData) * modelData.vertices.size());

	//// 頂点バッファビューを生成する
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//// リソースの先頭アドレスから
	//vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();

	//// 使用するリソースのサイズは頂点３つ分のサイズ
	//vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	//// １頂点あたりのサイズ
	//vertexBufferView.StrideInBytes = sizeof(VertexData); 

	//// 頂点リソースにデータを書き込む
	//VertexData* vertexData = nullptr;

	//// 書き込むためのアドレスを取得
	//vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData)* modelData.vertices.size());


	//// Sprite用のリソースを作る
	////ID3D12Resource* vertexResourceSprite = CreateBufferResource(device, sizeof(VertexData) * 4);
	////ResourceObject  vertexResourceSprite = CreateBufferResource(device, sizeof(VertexData) * 4);
	//Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResourceSprite = CreateBufferResource(device.Get(), sizeof(VertexData) * 4);

	//// 頂点バッファービューを生成する
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};

	//// リソースの先頭アドレスから使う
	//vertexBufferViewSprite.BufferLocation = vertexResourceSprite.Get()->GetGPUVirtualAddress();

	//// 使用するリソースのサイズは頂点6つ分のサイズ
	//vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 4;

	//// 1頂点当たりのサイズ
	//vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);

	//VertexData* vertexDataSprite = nullptr;

	//vertexResourceSprite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	//// 1枚目の三角形
	//vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f }; // 左下
	//vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	//vertexDataSprite[0].normal = { 0.0f,0.0f,-1.0f };

	//vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f }; // 左上
	//vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	//vertexDataSprite[1].normal = { 0.0f,0.0f,-1.0f };

	//vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f }; // 右下
	//vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	//vertexDataSprite[2].normal = { 0.0f,0.0f,-1.0f };

	////// 2枚目の三角形
	////vertexDataSprite[3].position = { 0.0f,0.0f,0.0f,1.0f }; // 左上
	////vertexDataSprite[3].texcoord = { 0.0f,0.0f };
	////vertexDataSprite[3].normal = { 0.0f,0.0f,-1.0f };

	//vertexDataSprite[3].position = { 640.0f,0.0f,0.0f,1.0f }; // 右上
	//vertexDataSprite[3].texcoord = { 1.0f,0.0f };
	//vertexDataSprite[3].normal = { 0.0f,0.0f,-1.0f };

	////vertexDataSprite[5].position = { 640.0f,360.0f,0.0f,1.0f }; // 右下
	////vertexDataSprite[5].texcoord = { 1.0f,1.0f };
	////vertexDataSprite[5].normal = { 0.0f,0.0f,-1.0f };

	//
	////ID3D12Resource* materialResourceSprite = CreateBufferResource(device, sizeof(Material));
	////ResourceObject materialResourceSprite = CreateBufferResource(device, sizeof(Material));
	//Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSprite = CreateBufferResource(device.Get(), sizeof(Material));

	//Material* materialDataSprite = nullptr;

	//materialResourceSprite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));

	//materialDataSprite->color = { 1.0f,1.0f,1.0f,1.0f };

	//materialDataSprite->enableLighting = false;

	//// sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	////ID3D12Resource* transformationMatrixResourceSprite = CreateBufferResource(device, sizeof(TransformationMatrix));
	////ResourceObject  transformationMatrixResourceSprite = CreateBufferResource(device, sizeof(TransformationMatrix));
	//Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResourceSprite = CreateBufferResource(device.Get(), sizeof(TransformationMatrix));

	//// データを書き込む
	//TransformationMatrix* transformationMatrixDataSprite = nullptr;

	//// 書き込むためのアドレスを取得
	//transformationMatrixResourceSprite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));

	//// 単位行列を書き込んでおく
	//transformationMatrixDataSprite->WVP = MakeIdentity4x4();
	//transformationMatrixDataSprite->World = MakeIdentity4x4();

	//Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//


	////ID3D12Resource* indexResourceSprite = CreateBufferResource(device, sizeof(uint32_t) * 6);
	////ResourceObject indexResourceSprite = CreateBufferResource(device, sizeof(uint32_t) * 6);
	//Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite = CreateBufferResource(device.Get(), sizeof(uint32_t) * 6);

	//D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};

	//// リソースの先頭アドレスから使う
	//indexBufferViewSprite.BufferLocation = indexResourceSprite.Get()->GetGPUVirtualAddress();

	//// 使用するリソースのサイズはインデックス6つ分のサイズ
	//indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;

	//// インデックスはuint32_tとする
	//indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	//// インデックスリソースにデータを書き込む
	//uint32_t* indexDataSprite = nullptr;
	//indexResourceSprite.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	//indexDataSprite[0] = 0; indexDataSprite[1] = 1; indexDataSprite[2] = 2;
	//indexDataSprite[3] = 1; indexDataSprite[4] = 3; indexDataSprite[5] = 2;

	//// metaDataを基にSRVを設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//srvDesc.Format = metadata.format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//// SRVを生成するDescriptorHeapの場所を決める
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	//// metaDataを基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	//srvDesc2.Format = metadata2.format;
	//srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャー
	//srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = GetCPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 2);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = GetGPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 2);

	//// 先頭はImGuiがつかっているのでその次を使う
	//textureSrvHandleCPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//textureSrvHandleGPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//// SRVの生成
	//device->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
	//device->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);


	///*---------------
	//初期化ここ
	//--------------*/

	//// Transform変数を作る

	//materialData->uvTransform = MakeIdentity4x4();

	//Transform uvTransformSprite{
	//	{1.0f,1.0f,1.0f},
	//	{0.0f,0.0f,0.0f},
	//	{0.0f,0.0f,0.0f},
	//};

	//Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	//Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-15.0f} };
	//bool useMonsterBall = true;

	MSG msg{};
	while (true)
	{
		if (winApp->ProcessMessage()) {
			break;
		}
		else
		{
			//////更新処理

			/*ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Checkbox("UseMonsterBall", &useMonsterBall);
			ImGui::ColorEdit4("Light Color", &directionLightData->color.x);
			ImGui::DragFloat3("Light Direction", &directionLightData->direction.x,0.01f);

			ImGui::DragFloat3("Plane.Rotate", &transform.rotate.x, 0.01f);*/

			input->Update();

			int a = 0;

			/*if (input->PushKey(DIK_RETURN)) {
				a++;

				Log(std::format("int a :{}", a));
			}*/

			/*ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);*/

			/*if (ImGui::IsItemEdited()) {
				directionLightData->direction = Normalize(directionLightData->direction);
			}
			ImGui::DragFloat("Light Intensity", &directionLightData->intensity,0.01f);*/

			//回転
			//transform.rotate.y += 0.03f;
			/*Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
			Matrix4x4 worldViewProjectionMatrix = Multiply( worldMatrix, Multiply(viewMatrix, projectionMatrix));
			wvpData->WVP = worldViewProjectionMatrix;
			wvpData->World = worldMatrix;*/
			                 

			//ImGui::InputFloat3("a", &worldMatrix.m[3][0]);

			// Sprite用のWorldViewprojectionMatrixを作る
			//Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
			//Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
			//Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(kClientWidth), float(kClientHeight), 0.0f, 100.0f);
			//Matrix4x4 worldViewProjectionMatrixSprite =Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
			//transformationMatrixDataSprite->WVP = worldViewProjectionMatrixSprite;
			//transformationMatrixDataSprite->World = worldMatrixSprite;

			//// UVTransform用の行列
			//Matrix4x4 uvTransformMatrix = MakeAffineMatrix(uvTransformSprite.scale, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });

			//uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));

			//uvTransformMatrix = Multiply(uvTransformMatrix, MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, uvTransformSprite.translate));

			//materialDataSprite->uvTransform = uvTransformMatrix;

			//ImGui::ShowDemoWindow();
			//ImGui::Render();

			directXC->PreDraw();

			//// RootSignatureを設定。PSOに設定してるけど別途設定が必要
			//commandList->SetGraphicsRootSignature(rootSignature.Get());
			//commandList->SetPipelineState(graphicsPipelineState.Get());  // PSOを設定
			//commandList->IASetVertexBuffers(0, 1, &vertexBufferView);  // VBVを設定

			////==============
			//// Sphere
			////==============
			//// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばいい
			//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//// マテリアルCBufferの場所を設定
			//commandList->SetGraphicsRootConstantBufferView(0, materialResource.Get()->GetGPUVirtualAddress());

			////commandList->SetGraphicsRootConstantBufferView(0, vertexResource->GetGPUVirtualAddress());

			//// wvp用のCbufferの場所を設定
			//commandList->SetGraphicsRootConstantBufferView(1, wvpResource.Get()->GetGPUVirtualAddress());

			//// SRVのDescriptorの先頭を設定。2はrootParameter[2]である。
			//commandList->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU2 : textureSrvHandleGPU);

			////==============
			//// Lightting
			////==============
			//commandList->SetGraphicsRootConstantBufferView(3, directionLightResource.Get()->GetGPUVirtualAddress());

			//// 描画
		 //   commandList->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

			////==============
			//// Sprite
			////==============
			//commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

			//commandList->SetGraphicsRootConstantBufferView(0, materialResourceSprite.Get()->GetGPUVirtualAddress());

			//commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite.Get()->GetGPUVirtualAddress());

			//commandList->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);

			//commandList->IASetIndexBuffer(&indexBufferViewSprite); // IBVの設定

			//

			////commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	  //      // ImGui

			//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());

			//// ImGui

			directXC->PostDraw();

		}
	}

	///
	/*
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();*/

	/// 

	//CloseHandle(fenceEvent);

	winApp->Finalize();

	delete input;
	delete winApp;

	return 0;
}