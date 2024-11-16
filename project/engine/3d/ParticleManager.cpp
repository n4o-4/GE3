#include "ParticleManager.h"
#include "TextureManager.h"
#include "MyMath.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager();
	}

	return instance;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager,Camera* camera)
{
	/*----------------------------------------------------------
	* 引数をメンバ変数に記録
	----------------------------------------------------------*/

	dxCommon_ = dxCommon;
	srvManager_ = srvManager;
	camera_ = camera;

	accelerationField.acceleration = { -8.0f,0.0f,0.0f };
	accelerationField.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField.area.max = { 1.0f,1.0f,1.0f };

	/*----------------------------------------------------------
	* ランダムエンジンの初期化
	----------------------------------------------------------*/

	std::random_device seedGenerator;
	randomEngine.seed(seedGenerator());

	/*----------------------------------------------------------
	* Pipelineの生成関数呼び出し
	----------------------------------------------------------*/

	CreatePipeline();

	/*----------------------------------------------------------
	* 頂点データの初期化関数呼び出し
	----------------------------------------------------------*/

	InitializeVertexData();

	/*----------------------------------------------------------
	* 頂点リソースの生成関数の呼び出し
	----------------------------------------------------------*/

	CreateVertexResource();

	/*----------------------------------------------------------
	* 頂点バッファビューの生成関数の呼び出し
	----------------------------------------------------------*/

	CreateVertexBufferView();

	/*----------------------------------------------------------
	* 頂点リソースに頂点データを書き込む関数の呼び出し
	----------------------------------------------------------*/

	WriteDataInResource();
}

void ParticleManager::Update()
{

	/*----------------------------------------------------------
	* ビルボードマトリクスを計算する関数の呼び出し
	----------------------------------------------------------*/
	calculationBillboardMatrix();



	Matrix4x4 viewMatrix = camera_->GetViewMatrix();

	Matrix4x4 projectionMatrix = camera_->GetProjectionMatrix();

	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups.begin(); particleGroupIterator != particleGroups.end();) {

		ParticleGroup* particleGroup = &(particleGroupIterator->second);

		particleGroupIterator->second.kNumInstance = 0;

		for (std::list<Particle>::iterator particleIterator = particleGroup->particles.begin(); particleIterator != particleGroup->particles.end();) {

			if (particleGroupIterator->second.kNumInstance < kNumMaxInstance) {

				if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
					particleIterator = particleGroup->particles.erase(particleIterator);
					continue;
				}

				if (IsCollision(accelerationField.area, (*particleIterator).transform.translate)) {
					(*particleIterator).velocity += accelerationField.acceleration * kDeltaTime;
				}

				(*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;
				(*particleIterator).currentTime += kDeltaTime;

				// 追加

				Matrix4x4 scaleMatrix;
				scaleMatrix.m[0][0] = (*particleIterator).transform.scale.x;
				scaleMatrix.m[0][1] = 0;
				scaleMatrix.m[0][2] = 0;
				scaleMatrix.m[1][0] = 0;
				scaleMatrix.m[1][1] = (*particleIterator).transform.scale.y;
				scaleMatrix.m[1][2] = 0;
				scaleMatrix.m[2][0] = 0;
				scaleMatrix.m[2][1] = 0;
				scaleMatrix.m[2][2] = (*particleIterator).transform.scale.z;
				scaleMatrix.m[3][3] = 1;

				Matrix4x4 worldMatrix = MakeIdentity4x4();

				worldMatrix.m[0][0] = scaleMatrix.m[0][0] * billboardMatrix.m[0][0];
				worldMatrix.m[0][1] = scaleMatrix.m[0][0] * billboardMatrix.m[0][1];
				worldMatrix.m[0][2] = scaleMatrix.m[0][0] * billboardMatrix.m[0][2];

				worldMatrix.m[1][0] = scaleMatrix.m[1][1] * billboardMatrix.m[1][0];
				worldMatrix.m[1][1] = scaleMatrix.m[1][1] * billboardMatrix.m[1][1];
				worldMatrix.m[1][2] = scaleMatrix.m[1][1] * billboardMatrix.m[1][2];

				worldMatrix.m[2][0] = scaleMatrix.m[2][2] * billboardMatrix.m[2][0];
				worldMatrix.m[2][1] = scaleMatrix.m[2][2] * billboardMatrix.m[2][1];
				worldMatrix.m[2][2] = scaleMatrix.m[2][2] * billboardMatrix.m[2][2];

				worldMatrix.m[3][0] = (*particleIterator).transform.translate.x;
				worldMatrix.m[3][1] = (*particleIterator).transform.translate.y;
				worldMatrix.m[3][2] = (*particleIterator).transform.translate.z;
				worldMatrix.m[3][3] = 1;

				Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

				particleGroup->instancingData[particleGroupIterator->second.kNumInstance].World = worldMatrix;
				particleGroup->instancingData[particleGroupIterator->second.kNumInstance].WVP = worldViewProjectionMatrix;

				float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);

				particleGroup->instancingData[particleGroupIterator->second.kNumInstance].color.x = (*particleIterator).color.x;
				particleGroup->instancingData[particleGroupIterator->second.kNumInstance].color.y = (*particleIterator).color.y;
				particleGroup->instancingData[particleGroupIterator->second.kNumInstance].color.z = (*particleIterator).color.z;

				particleGroup->instancingData[particleGroupIterator->second.kNumInstance].color.w = alpha;

				++particleGroupIterator->second.kNumInstance;
			}

			++particleIterator;
		}

		++particleGroupIterator;
	}
}

void ParticleManager::Draw(std::string filePath)
{
	// ルートシグネチャを設定
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	// PSOを設定
	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());

	// プリミティブトポロジーを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// VBVを設定
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 
	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups.begin(); particleGroupIterator != particleGroups.end();) {

		//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0,particleGroupIterator->second.)

		materialResource =dxCommon_->CreateBufferResource(sizeof(Material));

		// マテリアルにデータを書き込む
		Material * materialData = nullptr;

		// 書き込むためのアドレスを取得
		materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

		// 三角色
		materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f, };
		materialData->enableLighting = false;
		materialData->uvTransform = MakeIdentity4x4();

		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource.Get()->GetGPUVirtualAddress());

		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(particleGroupIterator->second.srvIndex));

		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2,srvManager_->GetGPUDescriptorHandle(particleGroupIterator->second.materialData.textureIndex));

		//dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(TextureManager::GetInstance()->GetTextureIndexByFilePath(filePath)));

		dxCommon_->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), particleGroupIterator->second.kNumInstance, 0, 0);

		++particleGroupIterator;
	}

}

void ParticleManager::CreateRootSignature()
{
	HRESULT hr;

	/*----------------------------------------------------------
	* RootSiganatureの生成
    ----------------------------------------------------------*/
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/*----------------------------------------------------------
	* RootParameterの生成
	----------------------------------------------------------*/

	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	/*----------------------------------------------------------
	* Samplerの生成
	----------------------------------------------------------*/

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイナリフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // 多くのMipMapを使う
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	/*----------------------------------------------------------
	* シリアライズしてバイナリにする
	----------------------------------------------------------*/

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	/*----------------------------------------------------------
	* バイナリを元RootSignatureを生成
	----------------------------------------------------------*/

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void ParticleManager::CreatePipeline()
{
	HRESULT hr;

	/*----------------------------------------------------------
	* RootSignature生成関数呼び出し
	----------------------------------------------------------*/

	CreateRootSignature();

	/*----------------------------------------------------------
	* InputElementの設定
	----------------------------------------------------------*/

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	/*----------------------------------------------------------
	* BlendStateの設定
	----------------------------------------------------------*/
	
	D3D12_BLEND_DESC blendDesc{};

	// 全ての要素数を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	/*----------------------------------------------------------
	* RasterizerStateの設定
	----------------------------------------------------------*/

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	/*----------------------------------------------------------
	* Shaderのコンパイル
	----------------------------------------------------------*/

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Particle.VS.hlsl", L"vs_6_0");

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Particle.PS.hlsl", L"ps_6_0");

	/*----------------------------------------------------------
	* DepthStencilStateの設定
	----------------------------------------------------------*/
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;

	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// 比較関数はLessEqual。近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	/*----------------------------------------------------------
	* GraphicsPipelineの設定
	----------------------------------------------------------*/

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 利用するトポロジー(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// どのように画面に色をつけるか
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	graphicsPipelineState = nullptr;
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void ParticleManager::InitializeVertexData()
{
	modelData.vertices.push_back({ {1.0f,1.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f} }); // 左上
	modelData.vertices.push_back({ {-1.0f,1.0f,0.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f,1.0f} }); // 右上
	modelData.vertices.push_back({ {1.0f,-1.0f,0.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f,0.0f} }); // 左下
	modelData.vertices.push_back({ {1.0f,-1.0f,0.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f,1.0f} }); // 左下
	modelData.vertices.push_back({ {-1.0f,1.0f,0.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f,1.0f} });// 右上
	modelData.vertices.push_back({ {-1.0f,-1.0f,0.0f,1.0f},{1.0f,1.0f},{0.0f,0.0f,1.0f} }); // 右下

	modelData.material.textureFilePath = "Reosurces/circle.png";
}

void ParticleManager::CreateVertexResource()
{
	// 頂点リソースを作る
	vertexResource = dxCommon_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

}

void ParticleManager::CreateVertexBufferView()
{
	// リソースの先頭アドレス
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();

	// 使用するリソースのサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void ParticleManager::WriteDataInResource()
{
	VertexData* vertexData;
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{
	// パーティクルグループの名前を検索

	// 無かった場合新しく作る

	if (particleGroups.find(name) == particleGroups.end()) {

		ParticleGroup newParticleGroup;

		newParticleGroup.materialData.textureFilePath = textureFilePath;

		// テクスチャ読み込み
		TextureManager::GetInstance()->LoadTexture(textureFilePath);

		// マテリアルデータにテクスチャのSRVインデックスを記録
		newParticleGroup.materialData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(newParticleGroup.materialData.textureFilePath);

		// インスタンシング用リソースの生成
		newParticleGroup.instancingResource = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
		
		newParticleGroup.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&newParticleGroup.instancingData));


		// インスタンシング用にSRVを確保してSRVインデックスを記録

		D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
		instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
		instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		instancingSrvDesc.Buffer.FirstElement = 0;
		instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
		instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

		newParticleGroup.srvIndex = srvManager_->Allocate();

		D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU = srvManager_->GetCPUDescriptorHandle(newParticleGroup.srvIndex);

		dxCommon_->GetDevice()->CreateShaderResourceView(newParticleGroup.instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);

		particleGroups[name] = newParticleGroup;

	}
}

// ビルボードマトリクスの計算
void ParticleManager::calculationBillboardMatrix()
{
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	billboardMatrix = Multiply(backToFrontMatrix, camera_->GetWorldMatrix());

	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
}

ParticleManager::Particle ParticleManager::MakeNewParticle(const Vector3& translate)
{
		// 新しいパーティクルの生成
		std::unique_ptr<Particle> newParticle;

		newParticle = std::make_unique<Particle>();

		//
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		Vector3 randomTranslate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };

		// transformの設定
		newParticle->transform.scale = { 0.9f,0.9f,0.9f };
		newParticle->transform.rotate = { 0.0f,0.0f,0.0f };
		newParticle->transform.translate = translate + randomTranslate;

		// velocityの設定
		newParticle->velocity = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };

		std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

		newParticle->color = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };

		std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

		newParticle->lifeTime = distTime(randomEngine);
		newParticle->currentTime = 0.0f;

		return *newParticle;
}

void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{

	if (particleGroups.find(name) != particleGroups.end()) {

		for (int currentCount = 0; currentCount < count;) {

			particleGroups.find(name)->second.particles.push_back(MakeNewParticle(position));

			++currentCount;

		}
	}

}

void ParticleManager::Finalize()
{

	delete instance;
	instance = nullptr;

}