// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit

// シャドウマップの有効フラグ
#define SHADOWMAP 1

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D Noise : register(t3); 
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float3 g_light_dir;
    float4 g_ambient_color;
    float dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    //ライト方向
    float3 lightDir = g_common.light[0].direction;

	//法線と定数バッファのライト方向の内積
    float3 dotL = dot(-g_light_dir, PSInput.normal);
    
    // 影の判定処理の初期値 (1.0 = 日向)
    float shadow = 1.0f;

    // ライト空間のXY座標（-1.0 ～ 1.0）を UV座標（0.0 ～ 1.0）に変換
    float2 shadowUV;
    shadowUV.x = PSInput.lightAtPos.x * 0.5f + 0.5f;
    shadowUV.y = -PSInput.lightAtPos.y * 0.5f + 0.5f; // DirectXのためY軸を反転

    // 描画中ピクセルのライトからの深度値
    float currentDepth = PSInput.lightAtPos.z;

    //シャドウマップの箱の「外側」および「奥（currentDepth > 1.0f）」を完全に除外
    if (shadowUV.x >= 0.0f && shadowUV.x <= 1.0f &&
        shadowUV.y >= 0.0f && shadowUV.y <= 1.0f &&
        currentDepth >= 0.0f && currentDepth <= 1.0f) //深度の限界値(1.0)チェック
    {
        // 描画中ピクセルのライトからの深度値
        float currentDepth = PSInput.lightAtPos.z;

        // シャドウマップから最も手前にある遮蔽物の深度値をサンプリング
        float closestDepth = shadowMap0Texture.Sample(shadowMap0Sampler, shadowUV).r;

        // 誤差による縞模様（シャドウアクネ）を防ぐためのバイアス値
        float bias = 0.005f;

        // 遮蔽物の方が手前にあれば（現在の深度の方が深ければ）影にする
        if (currentDepth - bias > closestDepth)
        {
            shadow = 0.5f; // 影の濃さ
        }
    }
    else
    {
        // シャドウマップの範囲外（遠くの景色やライトの届かない奥）は強制的に日向にする
        shadow = 1.0f;
    }
    
    
	//モデルからの色に乗算
    float3 ret = (color.rgb * g_color.rgb * dotL * shadow) + g_ambient_color.rgb;
    
    return float4(ret, color.a);
}