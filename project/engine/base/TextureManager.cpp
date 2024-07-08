#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;

// ImGuiで0番目を使用するため、1晩から使用
uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}

	return instance;
}

void TextureManager::Initialize(DirectXCommon* dxCommon,SrvManager* srvManager)
{
	// SRVの数と同数
	textureDatas.reserve(SrvManager::kMaxSRVCount);
	
	dxCommon_ = dxCommon;

	srvManager_ = srvManager;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::LoadTexture(const std::string& filePath)
{

	//if (it != textureDatas.end()) {
	//	// 読み込み済みなら早期return
	//	return;
	//}

	assert(srvManager_->CheckSrvCount());

	// テクスチャ枚数上限チェック
	assert(textureDatas.size() + kSRVIndexTop < SrvManager::kMaxSRVCount);

	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// テクスチャーデータを追加
	//textureDatas.resize(textureDatas.size() + 1);

	// テクスチャデータを追加して書き込む
	TextureData& textureData = textureDatas[filePath];

	// 追加したテクスチャデータの参照を取得する
	//TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);

	dxCommon_->UploadTextureData(textureData.resource, mipImages);

	// テクスチャデータの要素番号をSRVのインデックスとする
	/*uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.srvHandleCPU = dxCommon_->GetCPUDescriptorHandle(dxCommon_->srvDescriptorHeap, dxCommon_->descriptorSizeSRV,srvIndex);
	textureData.srvHandleGPU = dxCommon_->GetGPUDescriptorHandle(dxCommon_->srvDescriptorHeap, dxCommon_->descriptorSizeSRV,srvIndex);*/

	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels); 

	// SRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	// 読み込み済みテクスチャデータを検索
	//auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [&](TextureData& textureData) {return textureData.filePath == filePath; });
	//if (it != textureDatas.end()) {
	//	// 読み込み済みなら要素番号を返す
	//	uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
	//	return textureIndex;
	//}

	if (textureDatas.contains(filePath)) {
		
	}

	//assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(std::string filePath)
{
	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(std::string filePath)
{
	// TODO: return ステートメントをここに挿入します
	TextureData& textureData = textureDatas[filePath];
	return textureData.metadata;

}
