// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
 
// IN
#define PS_INPUT VertexToPixelShadow
 
SamplerState g_SrcSampler : register(s0);
Texture2D g_SrcTexture : register(t0);
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
 
	// Z値(深度、カメラから離れているほど1.0)
    float4 shadowDepth;
    shadowDepth.rgb = PSInput.svPosRead.z;
    shadowDepth.a = 1.0f;
 
	// 完全透過は描画しない
    float4 color = g_SrcTexture.Sample(g_SrcSampler, PSInput.uv);
    if (color.a < 0.0001f)
    {
        discard;
    }
 
    return shadowDepth;
 
}