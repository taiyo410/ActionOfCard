#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット0番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_rate;
    float3 dummy_;
}

// 描画するテクスチャ
Texture2D maskTex : register(t1);
//// サンプラー：適切な色を決める処理
//SamplerState g_SrcSampler : register(s0);
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // テクスチャ座標を取得
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    //テクスチャの反転色を取得
    float3 reverseCol = 1.0f - srcCol.rgb;

    //マスク画像の色割合を取得
    float4 maskCol = maskTex.Sample(texSampler, PSInput.uv);

    // UV座標とテクスチャを参照して、最適な色を取得する
    float maskRate = (maskCol.r + maskCol.g + maskCol.b) / (3.0f);
    
    float alpha = maskRate;
    
    //白と黒を反転させる
    alpha = 1.0f - alpha;

    //-1.0～0.0fにして、黒も完全描画させないようにする
    alpha = alpha - 1.0f;

    //g_rateに2.0をかけることで完全描画できてない黒にrateで色を調整できるようにする
    alpha = alpha + (2.0f * g_rate);
    
    // 0.0 ～ 1.0 に収める
    alpha = saturate(alpha);

    // 元の色 → 反転色
    float3 result = lerp(srcCol.rgb, reverseCol, alpha);
    
    return float4(result, alpha);
}