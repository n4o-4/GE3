#include "object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
    float3 specularColor;
};

struct DirectionLight
{
    float4 color;
    float3 direction;
    float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);

ConstantBuffer<DirectionLight> gDirectionalLight : register(b1);

Texture2D<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    if (gMaterial.enableLighting != 0)
    {
        float3 toEye = normalize(input.cameraPosition - input.worldPosition);
        
        float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        
        float RdotE = dot(reflectLight, toEye);
        
        float specularPow = pow(saturate(RdotE), gMaterial.shininess);
        
        // ‘O‚©‚ç‚ ‚Á‚½‚â‚Â«««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
        
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;  
        
        // ‘O‚©‚ç‚ ‚Á‚½‚â‚Âªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª
        
        // ŠgU”½Ë
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        // ‹¾–Ê”½Ë
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
        
        output.color.rgb = diffuse + specular;
        
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}


