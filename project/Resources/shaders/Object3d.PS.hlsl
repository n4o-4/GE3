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

struct SpotLight
{
    float4 color; // ライトの色
    float3 position; // ライトの位置
    float intensity; // 輝度
    float3 direction; // ライトの方向
    float distance; // ライトの届く最大距離
    float decay; // 減衰率
    float cosAngle; // スポットライトの余弦
    float cosFalloffStart;
};

struct LightDatas
{
    DirectionLight directionalLights[16];
    PointLight pointLights[16];
    SpotLight spotLights[16];
    
    int dLightCount;
    int pLightCount;
    int sLightCount;
};

ConstantBuffer<Material> gMaterial : register(b0);

ConstantBuffer<LightDatas> gLightDatas : register(b1);

Texture2D<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

float3 CalculationDirectionalLight(VertexShaderOutput input)
{
    float3 resultColor = { 0.0f, 0.0f, 0.0f };
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    float3 toEye = normalize(input.cameraPosition - input.worldPosition);
    
    
         // Phong Reflection↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
        
        //float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        
        //float RdotE = dot(reflectLight, toEye);
        
        //float specularPow = pow(saturate(RdotE), gMaterial.shininess);
        
        // Phong Reflection↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
        
        
        
        // BlinnPhong Reflection↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

    for (int i = 0; i < gLightDatas.dLightCount;)
    {
        float3 halfVector = normalize(-gLightDatas.directionalLights[i].direction + toEye);
        
        float NDotH = dot(normalize(input.normal), halfVector);
        
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
        
        // BlinnPhong Reflection↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
        
        float NdotL = dot(normalize(input.normal), -gLightDatas.directionalLights[i].direction);
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;  
        
        
        // 拡散反射
        float3 directionalLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gLightDatas.directionalLights[i].color.rgb * cos * gLightDatas.directionalLights[i].intensity;
        
        // 鏡面反射
        float3 directionalLight_Specular = gLightDatas.directionalLights[i].color.rgb * gLightDatas.directionalLights[i].intensity * specularPow * gMaterial.specularColor;
    
        resultColor += directionalLight_Diffuse + directionalLight_Specular;
        
        i++;

    }
    
    return resultColor;
}

float3 CalculationPointLight(VertexShaderOutput input)
{
    float3 resultColor = { 0.0f, 0.0f, 0.0f};
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float3 toEye = normalize(input.cameraPosition - input.worldPosition);
  
        
    for (int i = 0; i < gLightDatas.pLightCount;)
    {
        float distance = length(gLightDatas.pointLights[i].position - input.worldPosition);
        
        float factor = pow(saturate(-distance / gLightDatas.pointLights[i].radius + 1.0), gLightDatas.pointLights[i].decay);
        
        float3 pointLightDirection = normalize(input.worldPosition - gLightDatas.pointLights[i].position);
        
        float3 pointLight_HalfVector = normalize(-pointLightDirection + toEye);
        
        float pointLight_NDotH = dot(normalize(input.normal), pointLight_HalfVector);
        
        float pointLight_SpecularPow = pow(saturate(pointLight_NDotH), gMaterial.shininess);
        
        float pointLight_NDotL = dot(normalize(input.normal), -pointLightDirection);
        
        float pointLight_Cos = pow(pointLight_NDotL * 0.5 + 0.5f, 2.0f);
       
        // 拡散反射
        float3 pointLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gLightDatas.pointLights[i].color.rgb * pointLight_Cos * gLightDatas.pointLights[i].intensity * factor;
        
        // 鏡面反射
        float3 pointLight_Specular = gLightDatas.pointLights[i].color.rgb * gLightDatas.pointLights[i].intensity * pointLight_SpecularPow * factor * gMaterial.specularColor;
    
        resultColor += pointLight_Diffuse + pointLight_Specular;
        
        ++i;
    }
   
    return resultColor;
}

float3 CalculationSpottLight(VertexShaderOutput input)
{
    float3 resultColor = { 0.0f, 0.0f, 0.0f };
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float3 toEye = normalize(input.cameraPosition - input.worldPosition);
   
    for (int i = 0; i < gLightDatas.sLightCount;)
    {
        float spotLight_Distance = length(gLightDatas.spotLights[i].position - input.worldPosition); // ポイントライトへの距離
        
        float attenuationFactor = pow(saturate(-spotLight_Distance / gLightDatas.spotLights[i].distance + 1.0), gLightDatas.spotLights[i].decay);
        
        float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gLightDatas.spotLights[i].position);
        
        float cosAngle = dot(spotLightDirectionOnSurface, gLightDatas.spotLights[i].direction);
        
        float falloffFactor = saturate((cosAngle - gLightDatas.spotLights[i].cosAngle) / (gLightDatas.spotLights[i].cosFalloffStart - gLightDatas.spotLights[i].cosAngle));
        
        float3 spotLight_HalfVector = normalize(-gLightDatas.spotLights[i].direction + toEye);
        
        float spotLight_NDotH = dot(normalize(input.normal), spotLight_HalfVector);
        
        float spotLight_SpecularPow = pow(saturate(spotLight_NDotH), gMaterial.shininess);
        
        float spotLight_NdotL = dot(normalize(input.normal), -gLightDatas.spotLights[i].direction);
        
        float spotLight_Cos = pow(spotLight_NdotL * 0.5f + 0.5f, 2.0f);
        
        // 拡散反射
        float3 spotLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gLightDatas.spotLights[i].color.rgb * gLightDatas.spotLights[i].intensity * falloffFactor * attenuationFactor * spotLight_Cos;
        
        // 鏡面反射
        float3 spotLight_Specular = gLightDatas.spotLights[i].color.rgb * gLightDatas.spotLights[i].intensity * spotLight_SpecularPow * falloffFactor * attenuationFactor * gMaterial.specularColor;
    
        resultColor += spotLight_Diffuse + spotLight_Specular;
        
        ++i;

    }
        
    return resultColor;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    if (gMaterial.enableLighting != 0)
    {
        float3 toEye = normalize(input.cameraPosition - input.worldPosition);
        
        float3 directionalLightColor = CalculationDirectionalLight(input);
        
        float3 pointLightColor = CalculationPointLight(input);
        
        float3 spotLightColor = CalculationSpottLight(input);
       
        output.color.rgb = directionalLightColor + pointLightColor + spotLightColor;
        
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}

