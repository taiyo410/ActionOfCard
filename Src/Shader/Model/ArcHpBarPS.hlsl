#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color_1;       //塗りつぶしたい色
    float g_hp_Per;         //Hp割合
    float g_hp_lerp;        //補間割合
    float g_start_arc;      //スタート角度(1.0:右から反時計回り    0.75:上から反時計回り)
    float g_goal_arc;
    float dummy;
}

Texture2D tex2 : register(t1); //テクスチャ
SamplerState texSampler2 : register(s1);
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float PI = 3.14159265;
    float TWO_PI = 3.14159265 * 2.0f;
    float2 uv = PSInput.uv;
    float4 srcCol = tex.Sample(texSampler, uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    //半円ゲージ
    float2 p = uv - float2(0.5f, 0.5f);
    
    //角度
    float angle = atan2(p.y, p.x);
    
    //開始位置を上からにする
    angle += PI * 0.5f;
    
    if(angle<0)
    {
        angle += TWO_PI;
    }
    else if (angle > TWO_PI)
    {
        angle -= TWO_PI;
    }
    float angle01 = angle / TWO_PI;
    
    if (angle01 > g_hp_lerp * g_start_arc)
    {
        discard;
    }
    if (angle01 < g_hp_Per * g_start_arc)
    {
        srcCol.rgb = g_color_1;
    }
    else if (angle01 < g_hp_lerp * g_start_arc)
    {
        srcCol.rgb = float3(1.0f, 0.0f, 0.0f);
    }

    return srcCol;
   
}