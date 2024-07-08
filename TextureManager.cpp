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

void TextureManager::Initialize(DirectXCommon* dxCommon)
{
	// SRVの数と同数
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
	
	dxCommon_ = dxCommon;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [&](TextureData& textureData) {return textureData.filePath == filePath; });

	if (it != textureDatas.end()) {
		// 読み込み済みなら早期return
		return;
	}

	// テクスチャ枚数上限チェック
	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kMaxSRVCount);

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
	textureDatas.resize(textureDatas.size() + 1);

	// 追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);

	dxCommon_->UploadTextureData(textureData.resource, mipImages);

	// テクスチャデータの要素番号をSRVのインデックスとする
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.srvHandleCPU = dxCommon_->GetCPUDescriptorHandle(dxCommon_->srvDescriptorHeap, dxCommon_->descriptorSizeSRV,srvIndex);
	textureData.srvHandleGPU = dxCommon_->GetGPUDescriptorHandle(dxCommon_->srvDescriptorHeap, dxCommon_->descriptorSizeSRV,srvIndex);

	/*textureData.srvHandleCPU.ptr += dxCommon_->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureData.srvHandleGPU.ptr += dxCommon_->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);*/

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
	auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [&](TextureData& textureData) {return textureData.filePath == filePath; });
	if (it != textureDatas.end()) {
		// 読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}

	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex)
{
	TextureData& textureData = textureDatas.at(textureIndex);
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(uint32_t textureIndex)
{
	// TODO: return ステートメントをここに挿入します
	TextureData& textureData = textureDatas.at(textureIndex);
	return textureData.metadata;

}
