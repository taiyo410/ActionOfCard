

#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_time;
    float2 g_resolution;
    float dummy;
}
float d; // kernel offset

struct Rect
{
    float2 bottom_left, top_right;
};
bool IsPointInsideRect(float2 _point, Rect rect)
{
    return (rect.bottom_left.x <= _point.x 
    && _point.x <= rect.top_right.x 
    && rect.bottom_left.y <= _point.y
    && _point.y <= rect.top_right.y);
}
float2 WalkPointOnRectPerimeter(float2 dim, float t)
{
    float aspect = dim.x / dim.y;
    float height = 1.0 / (2.0 * (aspect + 1.0));
    float width = height * aspect;
    
    float tx = frac(t);
    float ty = frac(t + height);
    
    return float2(
        tx < 0.5 ?
        min(tx, width) :
        max(width + 0.5 - tx, 0.0),
        
        ty < 0.5 ?
        min(ty, height) :
        max(height + 0.5 - ty, 0.0)
    ) * dim.x / width;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    
    const float pi = 3.1415926535;
    const float4 center_color = float4(0.0, 0.0, 0.0, 0.0);
    const float4 border_color1 = float4(0.8, 0.2, 0.3, 1.0);
    const float4 border_color2 = float4(1.0, 0.9, 0.2, 1.0);
    
    float2 uv = PSInput.uv;
    float2 frag_coord = uv;
    
    // 画面の大きさに基づく半径
    float radius = (g_resolution.x + g_resolution.y) * 0.5;
    // ボーダーの太さ
    float border = g_resolution.y * 0.0154 + 0.06;
    // 中央の矩形
    Rect center_rect;
    center_rect.bottom_left = float2(border, border); // 左下
    center_rect.top_right = g_resolution - float2(border, border); // 右上
    float2 circle_center = WalkPointOnRectPerimeter(g_resolution.xy, g_time / 4.0); // 円の中心位置
    
    float4 frag_color = center_color;

    // 中央の矩形の外側の場合
    if (!IsPointInsideRect(frag_coord, center_rect))
    {
        // 円の中心からの距離に基づく補間係数
        float mix_factor = min(1.0, distance(circle_center, frag_coord) / radius);
        mix_factor = (1.0 - sin((mix_factor + 0.5) * pi)) * 0.5;
        frag_color = lerp(border_color1, border_color2, mix_factor);
    }
    if(frag_color.a < 0.01)
    {
        discard;
    }
    return frag_color;
}