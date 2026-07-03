//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"
//#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

//IN
#define PS_INPUT VertexToPixelShadow

// 法線マップ有効フラグ
#define BUMPMAP 1

// サブテクスチャの有効フラグ
#define SUBTEXTUREMODE 7

// シャドウマップの有効フラグ
#define SHADOWMAP 1

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// スポットライトの角度
static float SPOT_LIGHT_ANGLE_COS = cos(radians(30.0f));

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 g_light_dir;         // ライト方向
    float dummy;
    
    //float3 g_ambient_color; // 環境光
    //float dummy2;
    
    //float3 g_spot_light_pos;    // スポットライト位置
    //float g_is_light;           // ライトの電源(0オフ、1オン)
    
    //float3 g_spot_light_dir;    // スポットライト方向
    //float dummy4;
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
    float NdotL = max(0.0f, dot(normal, -lightDir));
    
    // 明部減衰（明るい色ほど環境光を弱める）
    //float3 ambientAttenuated = CalculateAmbientAttenuation(material, g_ambient_color);
    
    // ディフューズ
    float3 diffuse = material * NdotL;
 
    // 最終カラー 
    float3 litColor = saturate(diffuse) * 0.8f;
  
    
    //// サブテクスチャ色の取得
    //float4 subTexColor = subTexture.Sample(subSampler, uv);
    //litColor += subTexColor.rgb;
          
    // フォグ適用
   // float3 foggedColor = ApplyFog(litColor, PSInput.fogFactor);
    
    // 電源がオンの場合
    // スポットライトの色計算
    //float3 spotLight = CalculateSpotLite(PSInput.world, g_spot_light_pos, g_spot_light_dir, PSInput.normal);

    // 色の加算(電源がオフの場合0乗算で追加値なし)
    //foggedColor += spotLight * g_is_light;

    // 影の影響力を取得
    return float4(litColor, texColor.a);
    float shadowFactor = ShadowCalculation(PSInput.lightAtPos, shadowMap0Texture, shadowMap0Sampler, PSInput.normal, g_light_dir);

    // 最終色に影の係数を乗算
    litColor.rgb *= shadowFactor;
    
    // 色の出力
    return float4(litColor, texColor.a);
}