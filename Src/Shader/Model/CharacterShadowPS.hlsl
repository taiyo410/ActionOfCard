//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"
//#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

//IN
#define PS_INPUT VertexToPixelShadow

// 法線マップ有効フラグ
#define BUMPMAP 1
#define USE_SPETEX 2

// シャドウマップの有効フラグ
#define SHADOWMAP 1

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 g_light_dir;     // ライト方向
    float g_shininess;      // スペキュラの輝き度
    
    float3 g_ambient_color; // 環境光
    float dummy2;
    
    //float3 g_spot_light_pos; // スポットライト位置
    //float g_is_light; // ライトの電源(0オフ、1オン)
    
    //float3 g_spot_light_dir; // スポットライト方向
    float4 dummy4;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
 
    // ベースカラー
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = texColor.rgb;
    
    // 法線計算
    float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    float3 normal = CalculateNormal(PSInput.tan, PSInput.bin, PSInput.normal, tanNormal);
    // 光の方向
    float3 lightDir = normalize(g_light_dir.xyz);
    float NdotL = max(0.0f, dot(tanNormal, -lightDir));
    
    // 環境光（暗いマテリアルを持ち上げる）
    float3 ambientBase = material * g_ambient_color;
    
    // ディフューズ
    float3 diffuse = material * NdotL;
    
    // 最終的なカラー
    float3 finalColor = saturate(diffuse + ambientBase);
    
    //// フォグ適用
    //float3 foggedColor = ApplyFog(finalColor, PSInput.fogFactor);
    
    //// スポットライトの色計算
    //float3 spotLight = CalculateSpotLite(PSInput.world, g_spot_light_pos, g_spot_light_dir, PSInput.normal);

    // 色の加算
    //foggedColor += spotLight * g_is_light;
    
    //// 影の影響力を取得
    float shadowFactor = ShadowCalculation(PSInput.lightAtPos, shadowMap0Texture, shadowMap0Sampler, PSInput.normal, lightDir);
    
    // 最終色に影の係数を乗算
    finalColor.rgb *= shadowFactor;
    
    // 色の出力
    return float4(finalColor, texColor.a);
}