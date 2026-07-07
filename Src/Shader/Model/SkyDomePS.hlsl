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
}
 
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{

    float2 uv = PSInput.uv;

    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
 
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
    return color * g_color;

}