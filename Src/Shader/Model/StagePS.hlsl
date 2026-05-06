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
    //float g_time;

    //float3 dummy;
}
 
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{

    float2 uv = PSInput.uv;

    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
 
    return color;

}
 