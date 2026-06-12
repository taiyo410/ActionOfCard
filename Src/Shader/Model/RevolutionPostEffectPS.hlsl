#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット0番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float g_rate;
    float3 dummy_;
}

//// 描画するテクスチャ
//Texture2D g_SrcTexture : register(t0);
//// サンプラー：適切な色を決める処理
//SamplerState g_SrcSampler : register(s0);
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //// テクスチャ座標を取得
    ////float4 srcCol =float4(0.0f,0.0f,0.0f,1.0f);
    //float4 srcCol = tex.Sample(texSampler, PSInput.uv);


    //// UV座標とテクスチャを参照して、最適な色を取得する
    //float gray = (srcCol.r + srcCol.g + srcCol.b) / (3.0f);
    
    //float alpha = gray;
    ////白と黒を反転させる
    //alpha = 1.0f - alpha;
    ////return float4(alpha, alpha, alpha, 1.0f);

    ////-1.0～0.0fにして、黒も完全描画させないようにする
    //alpha = alpha - 1.0f;

    ////g_rateに2.0をかけることで完全描画できてない黒にrateで色を調整できるようにする
    //alpha = alpha + (2.0f * g_rate);

    //return float4(0.0f, 0.0f, 0.0f, alpha);
    //return srcCol * g_color;
    
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}