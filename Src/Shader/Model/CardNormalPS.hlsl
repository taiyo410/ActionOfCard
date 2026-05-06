#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)

cbuffer cbParam : register(b4)
{
    float4 g_color;
    float g_fog_Pow;
    float g_time;
    float2 dummy;
}

float lookup(float2 uv, float2 texelSize, float d, float dx, float dy)
{
    float2 offsetUV = uv + float2(dx, dy) * texelSize * d;
    float4 c = tex.Sample(texSampler, offsetUV);
    return dot(c.rgb, float3(0.2126, 0.7152, 0.0722));
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    
    float2 uv = PSInput.uv;
    float4 srcCol = tex.Sample(texSampler, uv);
    
    if (srcCol.a < 0.01)
    {
        discard;
    }
    
    //白いモヤを付ける
    float area = sin(g_time * 1.0);
    area = area * g_fog_Pow;
    area = abs(area);
    srcCol.rgb += area;
    
    return srcCol + g_color;
  
}

