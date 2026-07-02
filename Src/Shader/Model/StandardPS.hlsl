// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"
Texture2D Noise : register(t3); 
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float3 g_light_dir;
    float4 g_ambient_color;
    float dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    //ライト方向
    float3 lightDir = g_common.light[0].direction;

	//法線と定数バッファのライト方向の内積
    float3 dotL = dot(-g_light_dir, PSInput.normal);

	//モデルからの色に乗算
    float3 ret = (color.rgb * g_color.rgb * dotL) + g_ambient_color.rgb;

    return float4(ret, color.a);
}