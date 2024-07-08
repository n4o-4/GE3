#include "Sprite.h"
#include "SpriteCommon.h"
#include "TextureManager.h"

void Sprite::Initialize(SpriteCommon* spriteCommon, std::string textureFilePath)
{
	// 引数で受け取ってメンバ変数に記録する
	this->spriteCommon = spriteCommon;

	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	// バッファリソース

	CreateVertexData();

	CreateIndexData();

	CreateMaterialData();

	CreateTransformationMatrixData();
}

void Sprite::Update()
{
	// 1枚目の三角形
	vertexData[0].position = { 0.0f,1.0f,0.0f,1.0f }; // 左下
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData[1].position = { 0.0f,0.0f,0.0f,1.0f }; // 左上
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData[2].position = { 1.0f,1.0f,0.0f,1.0f }; // 右下
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };

	vertexData[3].position = { 1.0f,0.0f,0.0f,1.0f }; // 右上
	vertexData[3].texcoord = { 1.0f,0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };

	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;

	transform.scale = { size.x,size.y };

	transform.translate = { position.x,position.y,0.0f };

	transform.rotate = { 0.0f,0.0f,rotation };

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix= MakeOrthographicMatrix(0.0f, 0.0f,WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;

	// UVTransform用の行列
	Matrix4x4 uvTransformMatrix = MakeAffineMatrix(uvTransform.scale, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeAffineMatrix( { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, uvTransform.translate));
	materialData->uvTransform = uvTransformMatrix;
}



void Sprite::Draw()
{
	spriteCommon->GetDxCommon()->commandList->IASetVertexBuffers(0, 1, &vertexBufferView);  // VBVを設定

	spriteCommon->GetDxCommon()->commandList->IASetIndexBuffer(&indexBufferView); // IBVの設定

	spriteCommon->GetDxCommon()->commandList->SetGraphicsRootConstantBufferView(0, materialResource.Get()->GetGPUVirtualAddress());

	spriteCommon->GetDxCommon()->commandList->SetGraphicsRootConstantBufferView(1, transformationResource.Get()->GetGPUVirtualAddress());

	spriteCommon->GetDxCommon()->commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));

	spriteCommon->GetDxCommon()->commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::CreateVertexData()
{
	vertexResource = spriteCommon->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();

	// 使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;

	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;

	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Sprite::CreateIndexData()
{
	indexResource = this->spriteCommon->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	// リソースの先頭アドレスから使う
	indexBufferView.BufferLocation = indexResource.Get()->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;

	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// インデックスリソースにデータを書き込む
	indexData = nullptr;

	indexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
}

void Sprite::CreateMaterialData()
{
	materialResource = this->spriteCommon->GetDxCommon()->CreateBufferResource(sizeof(Material));

	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	materialData->color = { 1.0f,1.0f,1.0f,1.0f };

	materialData->enableLighting = false;

	materialData->uvTransform = MakeIdentity4x4();
}

void Sprite::CreateTransformationMatrixData()
{
	transformationResource = this->spriteCommon->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	// データを書き込むためのアドレスを取得
	transformationResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	//　単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();
}