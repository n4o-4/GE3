#include "LineDrawerBase.h"

void LineDrawerBase::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	pipeline_ = std::make_unique<Pipeline>();

	CreateRootSignature();

	CreatePipellineState();

}

void LineDrawerBase::Update()
{
	uint32_t instanceNum = 0;

	for (std::list<Line>::iterator lineIterator = lineObject_.lines.begin(); lineIterator != lineObject_.lines.end();) {
		if (instanceNum >= kMaxLines) {
			break; // 最大インスタンス数に達したら終了
		}

		//レンダリングパイプライン
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform.transform.scale, transform.transform.rotate, transform.transform.translate);
		lineResource_->instancingData[instanceNum].matWorld = worldMatrix;
		lineResource_->instancingData[instanceNum].position[0] = (*lineIterator).position[0];
		lineResource_->instancingData[instanceNum].position[1] = (*lineIterator).position[1];
		lineResource_->instancingData[instanceNum].color = (*lineIterator).color;

		//次のインスタンスへ
		++instanceNum;
		//次のイテレータに進む
		++lineIterator;
	}
}

void LineDrawerBase::Draw()
{

}

void LineDrawerBase::CreateLineResource()
{
	lineResource_ = std::make_unique<LineResource>();

	lineResource_->vertexBufferView.BufferLocation = lineResource_->vertexResource->GetGPUVirtualAddress();
	lineResource_->vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * 2);
	lineResource_->vertexBufferView.StrideInBytes = sizeof(VertexData);

	lineResource_->vertexResource = dxCommon_->CreateBufferResource(sizeof(VertexData) * 2);
	lineResource_->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&lineResource_->vertexData));
	std::memcpy(lineResource_->vertexData, vertices, sizeof(VertexData) * 2);
	
	lineResource_->instancingResource = dxCommon_->CreateBufferResource(sizeof(LineForGPU) * kMaxLines);
	lineResource_->instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&lineResource_->instancingData));

	transform.transform.translate = { 0.0f,0.0f,0.0f };
}

void LineDrawerBase::CreateRootSignature()
{
	HRESULT hr;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Tableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド

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

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline_->rootSignature));
	assert(SUCCEEDED(hr));
}

void LineDrawerBase::CreatePipellineState()
{
	HRESULT hr;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "VERTEXINDEX";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32_SINT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

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

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/PrimitiveDrawerVS.hlsl", L"vs_6_0");

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/PrimitiveDrawerPS.hlsl", L"ps_6_0");

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
	* BlendStateの設定
	----------------------------------------------------------*/

	D3D12_BLEND_DESC blendDesc{};

	// 全ての要素数を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].BlendEnable = FALSE;

	/*----------------------------------------------------------
	* GraphicsPipelineの設定
	----------------------------------------------------------*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = pipeline_->rootSignature.Get();
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
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipeline_->pipelineState));
	assert(SUCCEEDED(hr));
}

void LineDrawerBase::CreateVertexResource()
{

}
