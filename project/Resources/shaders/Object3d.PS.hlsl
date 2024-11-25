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

struct PointLight
{
    float4 color; // ライトの色
    float3 position; // ライトの色
    float intensity; // 輝度
    float radius; // ライトの届く最大距離
    float decay; // 減衰率
};

ConstantBuffer<Material> gMaterial : register(b0);

ConstantBuffer<DirectionLight> gDirectionalLight : register(b1);

ConstantBuffer<PointLight> gPointLight : register(b2);

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
        
         // Phong Reflection↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
        
        //float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        
        //float RdotE = dot(reflectLight, toEye);
        
        //float specularPow = pow(saturate(RdotE), gMaterial.shininess);
        
        // Phong Reflection↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
        
        
        
        // BlinnPhong Reflection↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        
        float NDotH = dot(normalize(input.normal), halfVector);
        
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
        // BlinnPhong Reflection↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
        
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;  
        
        
        // 拡散反射
        float3 directionalLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        // 鏡面反射
        float3 directionalLight_Specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
        
        
        // PointLight
        
        float distance = length(gPointLight.position - input.worldPosition);
        
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay);
        
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        
        float3 pointLight_HalfVector = normalize(-pointLightDirection + toEye);
        
        float pointLight_NDotH = dot(normalize(input.normal), pointLight_HalfVector);
        
        float pointLight_SpecularPow = pow(saturate(pointLight_NDotH), gMaterial.shininess);
        
        float pointLight_NDotL = dot(normalize(input.normal), -pointLightDirection);
        
        float pointLight_Cos = pow(pointLight_NDotL * 0.5 + 0.5f, 2.0f);
        
        
        float3 pointLight_Specular = gPointLight.color.rgb * gPointLight.intensity * specularPow * factor * gMaterial.specularColor;
    
        // 拡散反射
        float3 pointLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
        
        
        output.color.rgb = directionalLight_Diffuse + directionalLight_Specular + pointLight_Specular + pointLight_Specular;
        
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
    

}


