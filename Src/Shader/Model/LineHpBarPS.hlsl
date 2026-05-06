#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color_1;
    float4 g_color_2;
    float g_hp_Per;
    float g_hp_lerp; 
    float2 dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
    float4 srcCol = tex.Sample(texSampler, uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    else if (uv.x > g_hp_lerp)
    {
        float t = uv.x;
        srcCol.rgb = lerp(g_color_1, g_color_2, t);
        
        //元の色より暗くする
        srcCol.rgb /= 3.0f;
    }
    if (uv.x < g_hp_Per)
    {
        float t = uv.x;
        srcCol.rgb = lerp(g_color_1, g_color_2, t);
        //srcCol.rgb = float3(1.0 * (1.0f - g_hp_Per), 1.0f, 0.0f);
    }
    else if (uv.x < g_hp_lerp)
    {
        srcCol.rgb = float3(1.0f, 0.0f, 0.0f);
    }
    
    return srcCol;
   
}