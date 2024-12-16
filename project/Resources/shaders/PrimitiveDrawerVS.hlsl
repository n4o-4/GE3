#include "PrimitiveDrawer.hlsli"

struct ViewProjection
{
    float3 worldPosition;
    float4x4 matViewProjection;
};

struct VertexShaderInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;
};

ConstantBuffer <ViewProjection> gViewProjection : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
     
    output.position = mul(input.position, gViewProjection.matViewProjection);
    
    output.color = input.color;
    
    return output;
}