#include "PrimitiveDrawer.hlsli"

struct LineForGPU
{
    float4x4 matWorld; // 4x4 のワールド行列
    float4 position[2]; // 始点と終点の座標 (Vector4 型の配列)
    float4 color; // 線の色 (Vector4 型)
};

struct ViewProjection
{
    float3 worldPosition;
    float4x4 matViewProjection;
};


StructuredBuffer<LineForGPU> gLineForGPU : register(t0);

ConstantBuffer<ViewProjection> gViewProjection : register(b0);

struct VertexShaderInput
{
    float4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    float4x4 world =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    
    float4x4 WVP = mul(world, gViewProjection.matViewProjection);
    
    output.position = mul(input.position, WVP);
    
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return output;
}