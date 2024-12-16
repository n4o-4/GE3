#include "PrimitiveDrawer.hlsli"

cbuffer TransformBuffer : register(b0)
{
    float4x4 worldViewProjection; // ワールド・ビュー・プロジェクション行列
}

// 入力頂点データ
struct VertexShaderInput
{
    float3 position : POSITION; // 頂点のワールド空間位置
    float4 color : COLOR; // 頂点ごとの色
};

// 頂点シェーダー本体
VertexShaderOutput VS(VertexShaderInput input)
{
    VertexShaderOutput output;

    // 頂点位置をクリップ空間に変換
    output.position = mul(float4(input.position, 1.0f), worldViewProjection);

    // 頂点の色をそのまま出力
    output.color = input.color;

    return output;
}