#include "../Common/Pixel/PixelShader2DHeader.hlsli"
cbuffer Params : register(b4)
{
    float g_time;
    float g_intensity; // 線の強さ倍率(0.0～1.0 推奨)
    float2 dummy;
};

float rand(float2 p)
{
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

const float PI = 3.14159265;

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    
    float2 uv = PSInput.uv;
    float2 center = float2(0.5, 0.5);

    float2 dir = uv - center;

// 楕円スケール
    float2 ellipse = float2(1.4, 0.9);

// 楕円距離（0 = 中心）
    float dist = length(dir / ellipse);

// 角度
    float angle = atan2(dir.y, dir.x);

// ★ 量子化した角度
    float slice = floor(angle * 32.0) / 32.0;

// ★ パキパキ時間
    float t = g_time * 25.0;

// ★ 不連続サイン波
    //float density = sin(slice * 40.0 + g_time * 25.0);
    float flow = frac(dist * 10.0 - g_time * 8.0);
// 線マスク（緩め）
    float lineMask = smoothstep(0.0, 0.2, flow);

// フェード
    float fade = saturate(1.0 - dist);
    
    float innerRadius = 0.3; // 空洞の大きさ
    //float hollow = step(innerRadius, dist);
    float hollow = smoothstep(innerRadius, innerRadius + 0.05, dist);

// 合成
    float roarLine = lineMask * fade * hollow * g_intensity;
    float4 col = tex.Sample(texSampler, uv);
    if(dist < innerRadius)
    {
        discard;
    }
    else
    {

        col.rgb += roarLine;
    }


    return col;
}