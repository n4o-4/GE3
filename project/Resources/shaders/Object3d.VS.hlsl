#include "object3d.hlsli"

struct TransformationMatrix
{
    float4x4 View;
    float4x4 Projection;
};

struct WorldMatrix
{
    float4x4 World;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

ConstantBuffer<WorldMatrix> gWorldMatrix : register(b1);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float4x4 WVP = mul(gWorldMatrix.World, mul(gTransformationMatrix.View,gTransformationMatrix.Projection)); // âûã}èàíu
    output.position = mul(input.position, WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gWorldMatrix.World));
    return output;
}