#include "PrimitiveDrawer.hlsli"

struct Transform
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

struct VertexShaderInput
{
    float4 position : POSITION0;
    int vertexIndex : VERTEXINDEX0;
};

StructuredBuffer<Transform> gTransform : register(t0);

ConstantBuffer <ViewProjection> gViewProjection : register(b0);

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
     
    output.position = mul(gTransform[instanceId].position[0], gViewProjection.matViewProjection);
    
    
    

    
    return output;
    
    
    VertexShaderOutput output;
    //もし頂点番号(始点か終点かのデータ)が0(始点)だったら
    if (input.vertexIndex == 0.0f)
    {
        output.position = mul(gLine[instanceId].start, mul(mul(gTransform[instanceId].World, gViewProjectionTransformationMatrix.View), gViewProjectionTransformationMatrix.Projection));
        
    }
    //頂点番号が1(終点)だったら
    else
    {
        output.position = mul(gLine[instanceId].end, mul(mul(gTransform[instanceId].World, gViewProjectionTransformationMatrix.View), gViewProjectionTransformationMatrix.Projection));
    }
    output.color = gLine[instanceId].color;
    return output;
}